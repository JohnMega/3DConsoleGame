
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <iostream>
#include <conio.h>
#include <cmath>
#include <thread>
#include <chrono>
#include <tuple>
#include <fstream>
#include <experimental/filesystem>
#include <filesystem>

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>

#pragma warning(disable: 4996)

#include "ModelsManager.h"
#include "Circle.h"
#include "Triangle.h"

std::vector<class AActor*> actors;
ModelsManager modelsManager;

HWND GetConsoleHwnd()
{
    HWND hwnd;
    wchar_t Old[200];
    GetConsoleTitle(Old, 200);
    SetConsoleTitle(L"Console");
    Sleep(40);
    hwnd = FindWindow(NULL, L"Console");
    SetConsoleTitle(Old);
    return hwnd;
}

RECT desktop_rect_;
HWND desktop_ = GetConsoleHwnd();

void SetWindowSize()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD size = { 0, 0 };

    SetConsoleScreenBufferSize(consoleHandle, size);
    SMALL_RECT rect = { 0, 0, 63, 25 };
    SetConsoleWindowInfo(consoleHandle, TRUE, &rect);
}

POINT NewMouseCoords;
POINT OldMouseCoords;

int WindowX()
{
    return desktop_rect_.right;
}

int WindowY()
{
    return desktop_rect_.bottom;
}

void ClipMouse()
{
    RECT r2;
    RECT rr;

    GetWindowRect(desktop_, &rr);

    SetRect(&r2, rr.left, rr.top, rr.right, rr.bottom);
    ClipCursor(&r2);
}

void SetMouseSettings()
{
    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

    SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);

    /*Запретить выделение консоли*/
    DWORD prev_mode;
    GetConsoleMode(hin, &prev_mode);
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));

    SetConsoleMode(hin, ENABLE_MOUSE_INPUT);
}

void BlockMouseMovement()
{
    SetCursorPos(WindowX() / 2, WindowY() / 2);
}

void GetNewMousePos()
{
    if (NewMouseCoords.x > desktop_rect_.right - 10 || NewMouseCoords.x < desktop_rect_.left + 10 || NewMouseCoords.y > desktop_rect_.bottom - 10
        || NewMouseCoords.y < desktop_rect_.top + 10)
    {
        BlockMouseMovement();
        OldMouseCoords = { WindowX() / 2, WindowY() / 2 };
    }
    GetCursorPos(&NewMouseCoords);
}

void showCursor()
{
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 1;
    cci.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

void hideCursor(int off = 1)
{
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = off;
    cci.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

void EnableANSICode()
{
#if defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

void SpritesPreLoadTextures()
{
    std::vector<std::string> preLoadTexturesNames;
    preLoadTexturesNames.push_back("Textures/missing_textures/missing_textures.bmp");
    preLoadTexturesNames.push_back("Textures/Obsolete/Obsolete.bmp");

    std::string path = std::experimental::filesystem::current_path().string();
    path += "\\Textures";

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        std::string fn;
        fn.insert(0, entry.path().string().c_str() + entry.path().string().find_last_of('\\') + 1);

        if (fn.find(".bmp") != std::string::npos)
        {
            preLoadTexturesNames.push_back("Textures/" + fn);
        }
    }

    preLoadTexturesNames.push_back("Textures/env_fire/env_fire");
    preLoadTexturesNames.push_back("Textures/SmokeStackFallback0/SmokeStackFallback0");
    preLoadTexturesNames.push_back("Textures/SmokeStackFallback1/SmokeStackFallback1");
    preLoadTexturesNames.push_back("Textures/SmokeStackFallback2/SmokeStackFallback2");
    preLoadTexturesNames.push_back("Textures/SmokeStackFallback3/SmokeStackFallback3");
    preLoadTexturesNames.push_back("Textures/env_glow/env_glow0.bmp");
    preLoadTexturesNames.push_back("Textures/env_glow/env_glow1.bmp");
    preLoadTexturesNames.push_back("Textures/env_glow/env_glow2.bmp");
    preLoadTexturesNames.push_back("Textures/env_glow/env_glow3.bmp");

    Circle::SpritesPreLoad(preLoadTexturesNames);
}

void TrianglePreLoadTextures()
{
    std::vector<std::string> preLoadTexturesNames;
    preLoadTexturesNames.push_back("Textures/missing_textures/missing_textures.bmp");

    std::string path = std::experimental::filesystem::current_path().string();
    path += "\\Textures";

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        std::string fn;
        fn.insert(0, entry.path().string().c_str() + entry.path().string().find_last_of('\\') + 1);

        if (fn.find(".bmp") != std::string::npos)
        {
            preLoadTexturesNames.push_back("Textures/" + fn);
        }
    }

    Triangle::PreLoadTextures(preLoadTexturesNames);
}

void PreLoad()
{
    SpritesPreLoadTextures();
    TrianglePreLoadTextures();
}

extern void setWall();
extern void MenuImpl();

int main()
{
    srand(time(NULL));
    EnableANSICode();

    hideCursor();
    setWall();
    SetWindowSize();
    GetWindowRect(desktop_, &desktop_rect_);
    ClipMouse();
    SetMouseSettings();

    GetCursorPos(&OldMouseCoords);

    PreLoad();
    MenuImpl();
}