#pragma once

#define LAST_MENU_CLASS_NAME MenuClass

#define GET_LAST_MENU_CLASS_NAME_INNER(ProgName) CheckAdd##ProgName##ToStorage
#define GET_LAST_MENU_CLASS_NAME(ProgName) GET_LAST_MENU_CLASS_NAME_INNER(ProgName)

#define START_OF_DECLARE_MENU_ENUM_TYPES(MenuTypeName) \
enum class MenuTypeName##_MENU_TYPE : char { 

#define DECLARE_MENU_ENUM_TYPE(EnumName) EnumName,

#define END_OF_DECLARE_MENU_ENUM_TYPES() \
	END_OF_LIST };

#define DECLARE_ADDITION_ENUM_FEATURES(MenuTypeName, VarMenuTypeName) \
MenuTypeName##_MENU_TYPE curr##VarMenuTypeName##MenuState; \
\
MenuTypeName##_MENU_TYPE& operator++( MenuTypeName##_MENU_TYPE& type ) \
{ \
	type = static_cast<MenuTypeName##_MENU_TYPE>(static_cast<int>(type) + 1); \
	if (type == MenuTypeName##_MENU_TYPE::END_OF_LIST) \
		type = static_cast<MenuTypeName##_MENU_TYPE>(0); \
	return type; \
}

#define DECLARE_NEW_MENU_TYPE_INNER(ProgName, MenuName, MenuTypeName, VarMenuTypeName, EnumName) \
bool isInBuildModeOn##ProgName = false; \
\
class GET_LAST_MENU_CLASS_NAME(ProgName) : public LAST_MENU_CLASS_NAME \
{ \
using Super = LAST_MENU_CLASS_NAME; \
public: \
	virtual void DrawMenuTypes() override \
	{ \
		LAST_MENU_CLASS_NAME::DrawMenuTypes(); \
		\
		if(currentMenuType == MENU_TYPES::##MenuTypeName##_MENU) \
		{ \
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { ProgName##Info.x, ProgName##Info.y }); \
				std::cout << #MenuName; \
		} \
	} \
	virtual void DrawMenuStars() override \
	{ \
		LAST_MENU_CLASS_NAME::DrawMenuStars(); \
			\
			if(currentMenuType == MENU_TYPES::##MenuTypeName##_MENU) \
			{ \
			if (curr##VarMenuTypeName##MenuState == MenuTypeName##_MENU_TYPE::##EnumName) \
			{ \
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { ProgName##Info.x - 2, ProgName##Info.y }); \
				std::cout << "*"; \
			}\
			else \
			{ \
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { ProgName##Info.x - 2, ProgName##Info.y }); \
				std::cout << " "; \
			} \
			} \
	} \
	virtual void ClearMenuTypes() override \
	{ \
		LAST_MENU_CLASS_NAME::ClearMenuTypes(); \
		\
		if(currentMenuType == MENU_TYPES::##MenuTypeName##_MENU) \
		{ \
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { ProgName##Info.x - 2, ProgName##Info.y }); \
			std::cout << "                         "; \
		} \
	} \
	virtual void CheckAddMenuTypesToStorage() override; \
};

#define DECLARE_NEW_MENU_TYPE(ProgName, MenuName, MenuTypeName, VarMenuTypeName, EnumName) \
    DECLARE_NEW_MENU_TYPE_INNER(ProgName, MenuName, MenuTypeName, VarMenuTypeName, EnumName)

#define DECLARE_FORMARD_DECLARATIONS_INNER(ProgName, MenuName, EnumName, XPos, YPos) \
struct \
{ \
short x = XPos; \
short y = YPos; \
} ProgName##Info;

#define DECLARE_FORMARD_DECLARATIONS(ProgName, MenuName, EnumName, XPos, YPos) \
	DECLARE_FORMARD_DECLARATIONS_INNER(ProgName, MenuName, EnumName, XPos, YPos)

#define DECLARE_MENU_CLASS() \
class MenuClass \
{ \
public: \
	virtual void DrawMenuTypes() { } \
	virtual void DrawMenuStars() { } \
	virtual void ClearMenuTypes() { } \
	virtual void CheckAddMenuTypesToStorage() { } \
};

/////////////////////////////////////////////////

enum class MENU_TYPES { GAME_MENU };
static MENU_TYPES currentMenuType = MENU_TYPES::GAME_MENU;
DECLARE_MENU_CLASS()


START_OF_DECLARE_MENU_ENUM_TYPES(GAME)
	DECLARE_MENU_ENUM_TYPE(NEW_GAME)
	DECLARE_MENU_ENUM_TYPE(CREATE_SERVER)
	DECLARE_MENU_ENUM_TYPE(CONNECT_TO_SERVER)
	DECLARE_MENU_ENUM_TYPE(CUSTOMIZATION)
	DECLARE_MENU_ENUM_TYPE(EXIT)
END_OF_DECLARE_MENU_ENUM_TYPES()

DECLARE_FORMARD_DECLARATIONS(NewGame, New Game, NEW_GAME, SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2 - 6)
DECLARE_FORMARD_DECLARATIONS(CreateServer, Create server, CREATE_SERVER, SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2 - 4)
DECLARE_FORMARD_DECLARATIONS(ConnectToServer, Connect to server, CONNECT_TO_SERVER, SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2 - 2)
DECLARE_FORMARD_DECLARATIONS(Customization, Customization, CUSTOMIZATION, SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2)
DECLARE_FORMARD_DECLARATIONS(Exit, Exit, EXIT, SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2 + 2)