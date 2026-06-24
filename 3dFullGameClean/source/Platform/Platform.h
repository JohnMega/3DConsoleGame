#pragma once
// ----------------------------------------------------------------------------
// Platform.h - cross-platform compatibility shim.
//
// On Windows this just pulls in the real <Windows.h>/<conio.h>/<WinSock2.h>,
// so Windows behaviour is byte-for-byte unchanged. On Linux/macOS it provides
// minimal shims for the Windows console, input, mouse and Winsock APIs the
// game uses, implemented with termios + ANSI escapes + POSIX sockets.
//
// ponytail: shims cover exactly the symbols this codebase touches, nothing more.
// ----------------------------------------------------------------------------

#if defined(_WIN32) || defined(_WIN64)

    #include <Windows.h>
    #include <conio.h>
    #include <WinSock2.h>
    #include <WS2tcpip.h>
    #include <iostream>
    #include <string>
    // Same input model as the original Windows code: read a token via std::cin.
    namespace platform { inline void readToken(std::string& out) { std::cin >> out; } }

#else // ----------------------------- POSIX -----------------------------------

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cerrno>
#include <csignal>
#include <string>
#include <deque>
#include <unordered_map>
#include <mutex>
#include <chrono>

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

// On Windows the engine relies on <Windows.h>'s global min()/max() macros, but
// it also calls std::min/std::max elsewhere -- so a macro would break those.
// A using-declaration makes the bare calls resolve to std:: without collision.
// ponytail: declarations (not `using namespace`); narrow and ODR-safe.
using std::min;
using std::max;
using std::isnan;   // engine calls bare isnan(); global on Windows, std:: here

// ------------------------------- Windows types -------------------------------
typedef void*    HANDLE;
typedef void*    HWND;
typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t  BYTE;
typedef int      BOOL;
typedef short    SHORT;

#ifndef TRUE
  #define TRUE 1
#endif
#ifndef FALSE
  #define FALSE 0
#endif
#ifndef NULL
  #define NULL 0
#endif

struct POINT { long x; long y; };
struct COORD { short X; short Y; };
struct RECT  { long left; long top; long right; long bottom; };
struct SMALL_RECT { short Left; short Top; short Right; short Bottom; };
struct CONSOLE_CURSOR_INFO { DWORD dwSize; BOOL bVisible; };

// Console mode flags (values mirror the Win32 ones; unused on POSIX but the
// game ORs/ANDs them, so they must exist and be distinct bits).
#define STD_OUTPUT_HANDLE (-11)
#define STD_INPUT_HANDLE  (-10)
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define ENABLE_MOUSE_INPUT                 0x0010
#define ENABLE_QUICK_EDIT_MODE             0x0040
#define ENABLE_EXTENDED_FLAGS              0x0080
#define SPI_SETCURSORS                     0x0057

// ------------------------------- input backend -------------------------------
// One owner of stdin. Each frame the game polls _kbhit/_getch (menu) AND
// GetAsyncKeyState (movement) AND GetCursorPos (mouse-look). All three read
// through this single drained state so they never steal bytes from each other.
namespace platform {

using clock_t_ = std::chrono::steady_clock;

inline std::mutex&                                   in_mutex()  { static std::mutex m;                          return m; }
inline std::deque<int>&                              key_queue() { static std::deque<int> q;                     return q; }
inline std::unordered_map<int, clock_t_::time_point>& held_map() { static std::unordered_map<int, clock_t_::time_point> m; return m; }

// virtual (accumulated) mouse position fed to GetCursorPos
inline long& mouse_vx() { static long v = 0; return v; }
inline long& mouse_vy() { static long v = 0; return v; }
inline bool& mouse_have_last() { static bool b = false; return b; }
inline int&  mouse_last_x()    { static int v = 0; return v; }
inline int&  mouse_last_y()    { static int v = 0; return v; }

// ponytail: terminal mouse-look reports in character cells; scale so dividing
// by the camera's /3..50 still yields motion. Tune if look feels fast/slow.
static const int   MOUSE_SENS   = 12;
// ponytail: a key counts as "held" for this long after its last repeat byte.
// Bridges keyboard auto-repeat gaps so WASD movement is continuous.
static const long  HELD_MS      = 160;

inline bool is_tty() { static int t = ::isatty(STDIN_FILENO); return t == 1; }

inline struct termios& orig_termios() { static struct termios t; return t; }

inline void restore_terminal()
{
    if (!is_tty()) return;
    ::tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios());
    ::fputs("\033[?1003l\033[?1006l\033[?25h", stdout);  // mouse off, cursor on
    ::fflush(stdout);
}

inline void init_once()
{
    static bool done = false;
    if (done) return;
    done = true;

    ::signal(SIGPIPE, SIG_IGN);                     // never die on send() to a dropped peer
    if (!is_tty()) return;                          // piped input (e.g. tests): leave cooked

    ::tcgetattr(STDIN_FILENO, &orig_termios());     // save for restore
    struct termios raw = orig_termios();
    // input side only: no canonical, no echo, no CR->NL, no flow control.
    // Output flags (OPOST/ONLCR) are left ON so std::cout newlines stay sane.
    raw.c_lflag &= ~(ICANON | ECHO | ISIG);
    raw.c_iflag &= ~(ICRNL | IXON);
    raw.c_cc[VMIN]  = 0;                             // non-blocking read
    raw.c_cc[VTIME] = 0;
    ::tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    // any-event mouse tracking + SGR extended coordinates
    ::fputs("\033[?1003h\033[?1006h", stdout);
    ::fflush(stdout);

    std::atexit(restore_terminal);
}

// Parse one SGR mouse report body "b;x;yM" / "b;x;ym" starting at s[i] (after
// the '<'). Returns index just past the terminating M/m, or npos on malformed.
inline size_t parse_mouse(const std::string& s, size_t i)
{
    int b = 0, x = 0, y = 0, field = 0;
    size_t j = i;
    for (; j < s.size(); ++j)
    {
        char c = s[j];
        if (c >= '0' && c <= '9') { int* t = field == 0 ? &b : field == 1 ? &x : &y; *t = *t * 10 + (c - '0'); }
        else if (c == ';') ++field;
        else if (c == 'M' || c == 'm') { ++j; break; }
        else return std::string::npos;
    }
    // x,y are 1-based terminal cells. Accumulate deltas into the virtual cursor.
    if (mouse_have_last())
    {
        mouse_vx() += (long)(x - mouse_last_x()) * MOUSE_SENS;
        mouse_vy() += (long)(y - mouse_last_y()) * MOUSE_SENS;
    }
    mouse_last_x() = x;
    mouse_last_y() = y;
    mouse_have_last() = true;
    return j;
}

inline void record_key(int code)
{
    key_queue().push_back(code);
    held_map()[std::toupper(code)] = clock_t_::now();   // VK letters == ASCII upper
}

inline void drain()
{
    init_once();
    if (!is_tty()) return;          // piped input belongs to getchar()/std::cin
    std::lock_guard<std::mutex> lk(in_mutex());

    std::string buf;
    char tmp[256];
    for (;;)
    {
        ssize_t n = ::read(STDIN_FILENO, tmp, sizeof(tmp));
        if (n > 0) buf.append(tmp, (size_t)n);
        else break;
    }

    for (size_t i = 0; i < buf.size(); )
    {
        unsigned char c = (unsigned char)buf[i];
        if (c == 27 && i + 1 < buf.size() && buf[i + 1] == '[')
        {
            if (i + 2 < buf.size() && buf[i + 2] == '<')        // SGR mouse
            {
                size_t e = parse_mouse(buf, i + 3);
                if (e == std::string::npos) { i += 2; }
                else i = e;
            }
            else if (i + 2 < buf.size())                         // arrow / other CSI
            {
                char a = buf[i + 2];
                if (a == 'C')      record_key(0x4D);            // ARROW_RIGHT_KEY
                else if (a == 'D') record_key(0x4B);            // ARROW_LEFT_KEY
                // up/down/others: ignored (unused by menus)
                i += 3;
            }
            else i += 1;
        }
        else if (c == 27) { record_key(27); ++i; }              // bare ESC
        else if (c == '\n') { record_key('\r'); ++i; }          // Enter -> '\r'
        else { record_key((int)c); ++i; }
    }
}

// Read one whitespace-delimited token, mirroring `std::cin >> str`. The engine
// mixed blocking std::cin (menu text fields) with non-blocking key polling on
// the same terminal -- a Windows-console model. Routing menu text entry through
// the one input owner here keeps the two from stealing each other's bytes, and
// echoes characters (raw mode has echo off).
inline void readToken(std::string& out)
{
    out.clear();
    if (!is_tty())                                   // pipe/redirect: like std::cin >>
    {
        int c = ::getchar();
        while (c != EOF && std::isspace(c)) c = ::getchar();
        while (c != EOF && !std::isspace(c)) { out.push_back((char)c); c = ::getchar(); }
        return;
    }
    for (;;)                                         // interactive terminal
    {
        drain();
        bool done = false;
        {
            std::lock_guard<std::mutex> lk(in_mutex());
            while (!key_queue().empty())
            {
                int k = key_queue().front();
                key_queue().pop_front();
                if (k == '\r' || k == '\n' || k == ' ' || k == '\t')
                {
                    if (!out.empty()) { done = true; break; }    // token complete
                }
                else if (k == 127 || k == 8)                     // backspace
                {
                    if (!out.empty()) { out.pop_back(); std::fputs("\b \b", stdout); }
                }
                else if (k >= 32 && k < 127)
                {
                    out.push_back((char)k);
                    std::putchar(k);                             // echo
                }
            }
        }
        std::fflush(stdout);
        if (done) return;
        ::usleep(1000);
    }
}

} // namespace platform

// ------------------------------- conio.h -------------------------------------
// On a pipe (non-tty: scripted/test input) we bypass the drain buffer and read
// stdin directly, so std::cin (used by the menu's map-name/IP entry) and _getch
// (menu navigation) can share stdin without stealing each other's bytes.
inline int _kbhit()
{
    if (!platform::is_tty())
    {
        fd_set fds; FD_ZERO(&fds); FD_SET(STDIN_FILENO, &fds);
        struct timeval tv{0, 0};
        return ::select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0 ? 1 : 0;
    }
    platform::drain();
    std::lock_guard<std::mutex> lk(platform::in_mutex());
    return platform::key_queue().empty() ? 0 : 1;
}

inline int _getch()
{
    if (!platform::is_tty())
    {
        int c = ::getchar();
        if (c == EOF) return -1;
        return c == '\n' ? '\r' : c;                            // Enter -> '\r'
    }
    for (;;)
    {
        platform::drain();
        {
            std::lock_guard<std::mutex> lk(platform::in_mutex());
            if (!platform::key_queue().empty())
            {
                int k = platform::key_queue().front();
                platform::key_queue().pop_front();
                return k;
            }
        }
        ::usleep(1000);                                         // block like Win _getch
    }
}

// ----------------------- GetAsyncKeyState ------------------------------------
// Returns 0x8000 (high bit set) while a key is considered held, matching the
// game's `& 0x8000` test.
inline SHORT GetAsyncKeyState(int vk)
{
    platform::drain();
    std::lock_guard<std::mutex> lk(platform::in_mutex());
    auto it = platform::held_map().find(vk);
    if (it == platform::held_map().end()) return 0;
    long age = (long)std::chrono::duration_cast<std::chrono::milliseconds>(
                   platform::clock_t_::now() - it->second).count();
    return age <= platform::HELD_MS ? (SHORT)0x8000u : 0;
}

// ------------------------------- console -------------------------------------
inline HANDLE GetStdHandle(int) { return (HANDLE)1; }

inline BOOL SetConsoleCursorPosition(HANDLE, COORD c)
{
    std::printf("\033[%d;%dH", (int)c.Y + 1, (int)c.X + 1);     // ANSI is 1-based row;col
    std::fflush(stdout);
    return TRUE;
}
inline BOOL SetConsoleMode(HANDLE, DWORD)             { return TRUE; }
inline BOOL GetConsoleMode(HANDLE, DWORD* m)          { if (m) *m = 0; return TRUE; }
inline BOOL SetConsoleScreenBufferSize(HANDLE, COORD) { return TRUE; }
inline BOOL SetConsoleWindowInfo(HANDLE, BOOL, const SMALL_RECT*) { return TRUE; }
inline BOOL FlushConsoleInputBuffer(HANDLE)           { return TRUE; } // ponytail: no-op; drain() owns input state
inline void SetConsoleCursorInfo(HANDLE, const CONSOLE_CURSOR_INFO* c)
{
    std::fputs(c && c->bVisible ? "\033[?25h" : "\033[?25l", stdout);
    std::fflush(stdout);
}

// title / window: meaningless in a terminal -> stubs (wide-char like Win32)
inline DWORD GetConsoleTitleW(wchar_t*, DWORD) { return 0; }
inline BOOL  SetConsoleTitleW(const wchar_t*)  { return TRUE; }
inline HWND  FindWindowW(const wchar_t*, const wchar_t*) { return (HWND)1; }
#define GetConsoleTitle GetConsoleTitleW
#define SetConsoleTitle SetConsoleTitleW
#define FindWindow      FindWindowW

// ------------------------------- window / mouse ------------------------------
// Report a huge window rect so the game's "mouse near edge -> warp to centre"
// branch never fires (we can't warp a terminal cursor; we accumulate deltas).
inline BOOL GetWindowRect(HWND, RECT* r)
{
    if (r) { r->left = 0; r->top = 0; r->right = 1 << 20; r->bottom = 1 << 20; }
    return TRUE;
}
inline BOOL SetRect(RECT* r, int l, int t, int ri, int b)
{
    if (r) { r->left = l; r->top = t; r->right = ri; r->bottom = b; }
    return TRUE;
}
inline void ClipCursor(const RECT*) {}
inline void SetCursorPos(int, int)  {}                          // no terminal cursor warp
inline BOOL GetCursorPos(POINT* p)
{
    platform::drain();
    std::lock_guard<std::mutex> lk(platform::in_mutex());
    if (p) { p->x = platform::mouse_vx(); p->y = platform::mouse_vy(); }
    return TRUE;
}
inline void SystemParametersInfo(unsigned, unsigned, void*, unsigned) {}

// ------------------------------- timing --------------------------------------
inline void Sleep(DWORD ms) { ::usleep((useconds_t)ms * 1000); }

// ------------------------------- Winsock -------------------------------------
typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR   (-1)
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr    SOCKADDR;
struct WSAData { int unused; };
typedef WSAData WSADATA;

#define MAKEWORD(a, b) ((WORD)(((a) & 0xff) | (((WORD)((b) & 0xff)) << 8)))
#define ZeroMemory(p, n) std::memset((p), 0, (n))

inline int    WSAStartup(WORD, WSAData*) { return 0; }
inline int    WSACleanup()               { return 0; }
inline int    WSAGetLastError()          { return errno; }
inline int    closesocket(SOCKET s)      { return ::close(s); }

#endif // _WIN32

// ------------------------------- clear screen --------------------------------
// The engine called system("cls") directly. That spawns a missing process on
// POSIX (and is slow everywhere). clearScreen() keeps cls on Windows and uses
// an ANSI home+erase elsewhere -- same visible effect, no subprocess.
#include <cstdio>
#include <cstdlib>
inline void clearScreen()
{
#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#else
    std::fputs("\033[2J\033[H", stdout);
    std::fflush(stdout);
#endif
}
