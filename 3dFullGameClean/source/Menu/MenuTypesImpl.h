#pragma once

#include <Windows.h>
#include <iostream>

#include "MenuTypesDeclares.h"
#include "GlobalVars.h"

DECLARE_ADDITION_ENUM_FEATURES(GAME, Game)

#undef LAST_MENU_CLASS_NAME 
#define LAST_MENU_CLASS_NAME GameMenuStarControllers
class GameMenuStarControllers : public MenuClass
{
public:
	virtual void DrawMenuStars() override
	{
		if (currentMenuType == MENU_TYPES::GAME_MENU) ++currGameMenuState;
	}
};

DECLARE_NEW_MENU_TYPE(NewGame, New Game, GAME, Game, NEW_GAME)
#undef LAST_MENU_CLASS_NAME 
#define LAST_MENU_CLASS_NAME GET_LAST_MENU_CLASS_NAME(NewGame)
void LAST_MENU_CLASS_NAME::CheckAddMenuTypesToStorage()
{
	Super::CheckAddMenuTypesToStorage();

	if (currGameMenuState == GAME_MENU_TYPE::NEW_GAME)
	{
		system("cls");
		setWall();

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 2 });
		std::cout << "Enter the map name";
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 1 });
		std::cin >> mapName;

		std::ifstream in(mapName);
		if (!in.is_open())
		{
			eraseWindowContent();
			DrawMenu();
			//DrawMenuStars();
			key = NOTHING_KEY;
			return;
		}

		LoadMap();
		currGameState = GAME_STATE::GAME;
	}
}

DECLARE_NEW_MENU_TYPE(CreateServer, Create server, GAME, Game, CREATE_SERVER)
#undef LAST_MENU_CLASS_NAME 
#define LAST_MENU_CLASS_NAME GET_LAST_MENU_CLASS_NAME(CreateServer)
void LAST_MENU_CLASS_NAME::CheckAddMenuTypesToStorage()
{
	Super::CheckAddMenuTypesToStorage();

	if (currGameMenuState == GAME_MENU_TYPE::CREATE_SERVER)
	{
		system("cls");
		setWall();

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 2 });
		std::cout << "Enter your IP adress";
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 1 });
		std::cin >> serverPartData.myServerIP;

		while (true)
		{
			system("cls");
			setWall();

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 2 });
			std::cout << "Enter the map name";
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 1 });
			std::cin >> mapName;

			std::ifstream in(mapName);
			if (!in.is_open())
			{
				eraseWindowContent();
				key = NOTHING_KEY;
				continue;
			}

			break;
		}

		LoadMap();

		serverPartData.characterActor = actors.back();

		std::thread* crServer = new std::thread(ServerCreate);
		std::thread* clDataHandle = new std::thread(ClientDataHandle);
		serverPartData.isServerCreate = true;

		currGameState = GAME_STATE::GAME;
	}
}

DECLARE_NEW_MENU_TYPE(ConnectToServer, Connect to server, GAME, Game, CONNECT_TO_SERVER)
#undef LAST_MENU_CLASS_NAME 
#define LAST_MENU_CLASS_NAME GET_LAST_MENU_CLASS_NAME(ConnectToServer)
void LAST_MENU_CLASS_NAME::CheckAddMenuTypesToStorage()
{
	Super::CheckAddMenuTypesToStorage();

	if (currGameMenuState == GAME_MENU_TYPE::CONNECT_TO_SERVER)
	{
		system("cls");
		setWall();

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 2 });
		std::cout << "Enter IP adress";
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 1 });
		std::cin >> clientPartData.serverIP;

		ClientConnection();
		std::thread* srDataHandle = new std::thread(ServerDataHandle);

		char mapNamePtr[256];
		recv(clientPartData.Connection, mapNamePtr, sizeof(mapNamePtr), NULL);
		std::string serverMapName = mapNamePtr;

		char serverDefaultModelName[256];
		recv(clientPartData.Connection, serverDefaultModelName, sizeof(serverDefaultModelName), NULL);
		modelsManager.SetDefaultModelName(std::string(serverDefaultModelName));

		std::ifstream in(serverMapName);
		if (!in.is_open())
		{
			clientPartData.isConnect = false;
			closesocket(clientPartData.Connection);

			eraseWindowContent();
			DrawMenu();
			//DrawMenuStars();
			key = NOTHING_KEY;
			return;
		}

		mapName = serverMapName;
		LoadMap();

		clientPartData.characterActor = actors.back();
		currGameState = GAME_STATE::GAME;
	}
}

DECLARE_NEW_MENU_TYPE(Customization, Customization, GAME, Game, CUSTOMIZATION)
#undef LAST_MENU_CLASS_NAME 
#define LAST_MENU_CLASS_NAME GET_LAST_MENU_CLASS_NAME(Customization)
void LAST_MENU_CLASS_NAME::CheckAddMenuTypesToStorage()
{
	Super::CheckAddMenuTypesToStorage();

	if (currGameMenuState == GAME_MENU_TYPE::CUSTOMIZATION)
	{
		Model* currModel;

		if (modelsManager.GetLoadedModels().size() == 0)
			for (size_t i = 0; i < modelsManager.GetPlayerModelsNames().size(); ++i)
			{
				modelsManager.AddModelToLoadedModels(new Model);
				currModel = modelsManager.GetLoadedModels()[i];
				currModel->AddNewAnimationPart(modelsManager.GetPlayerModelsNames()[i] + "/" + modelsManager.GetPlayerModelsNames()[i] + "Idle");
				currModel->SetCentreCoord() = { 0,0,0 };
				currModel->UpdateRange();

				currModel->SetRenderMode(false);
			}

		currModel = modelsManager.GetLoadedModels()[0];
		currModel->SetRenderMode(true);
		camActor.AddCamera(new ASphereCamera({ currModel->GetCentreCoord().x, currModel->GetCentreCoord().y, currModel->GetCentreCoord().z + (currModel->GetModelRange().zEnd - currModel->GetModelRange().zStart) / 1.5f }, currModel));
		AddActorToStorage(actors, &camActor);
		for (size_t i = 0; i < modelsManager.GetLoadedModels().size(); ++i) AddActorToStorage<ABaseActor>(actors, modelsManager.GetLoadedModels()[i]);
		AddActorToStorage<ALightActor>(actors, new Light(5000, { 20,14,15 }), new Circle({ 20,14,15 }, { 1,0,0 }, "Textures/env_glow/env_glow0.bmp", 1, 5));

		currGameState = GAME_STATE::GAME;
	}
}

DECLARE_NEW_MENU_TYPE(Exit, Exit, GAME, Game, EXIT)
#undef LAST_MENU_CLASS_NAME 
#define LAST_MENU_CLASS_NAME GET_LAST_MENU_CLASS_NAME(Exit)
void LAST_MENU_CLASS_NAME::CheckAddMenuTypesToStorage()
{
	Super::CheckAddMenuTypesToStorage();

	if (currGameMenuState == GAME_MENU_TYPE::EXIT)
	{
		exit(0);
	}
}


MenuClass* generalMenuClass = new LAST_MENU_CLASS_NAME;
#undef LAST_MENU_CLASS_NAME
#define LAST_MENU_CLASS_NAME MenuClass