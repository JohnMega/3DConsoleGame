
#include <fstream>
#include <thread>
#include <iostream>
#include <conio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "Menu.h"
#include "MenuTypesDeclares.h"
#include "GlobalVars.h"
#include "Maths.h"
#include "ACamera.h"
#include "StaticMesh.h"
#include "ASpectatorCamera.h"
#include "ASphereCamera.h"
#include "Pixel.h"
#include "Parallelepiped.h"
#include "Pyramid.h"
#include "Light.h"
#include "AActor.h"
#include "Delegate.h"
#include "ATriggerActor.h"
#include "ABaseActor.h"
#include "ALightActor.h"
#include "ACharacterActor.h"
#include "ASmokeActor.h"
#include "Render.h"
#include "MapLoader.h"
#include "Server.h"
#include "Client.h"
#include "MenuTypesImpl.h"

extern std::vector<class AActor*> actors;
extern ModelsManager modelsManager;

extern void GetNewMousePos();

void eraseWindowContent()
{
	for (size_t i = 0; i < SCREEN_HEIGHT - 2; ++i)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 1, (short)i });
		for (size_t j = 1; j < SCREEN_WIDTH - 1; ++j)
			std::cout << " ";
	}
}

void setWall()
{
	for (size_t i = 0; i < SCREEN_HEIGHT; ++i)
	{
		std::cout << '#';

		for (size_t j = 1; j < SCREEN_WIDTH - 1; ++j)
		{
			std::cout << ((!i || i == SCREEN_HEIGHT - 1) ? '#' : ' ');
		}

		std::cout << '#' << '\n';
	}
}

void DrawMenu()
{
	generalMenuClass->DrawMenuTypes();
}

void DrawMenuStars()
{
	generalMenuClass->DrawMenuStars();
}

void OnGameKeyCheck()
{
	auto ServerPartCleanup = []()
		{
			if (clientPartData.isConnect)
			{
				clientPartData.isConnect = false;
				clientPartData.isClientDataReadyForCleanup = true;
			}

			if (serverPartData.isServerCreate)
				serverPartData.ServerDataCleanup();
		};

	if (key == ESC_KEY)
	{
		int size = actors.size();
		for (size_t i = 0; i < size; ++i)
			actors.pop_back();

		for (size_t i = 0; i < modelsManager.GetLoadedModels().size(); ++i)
			modelsManager.GetLoadedModels()[i]->SetRenderMode(false);

		camActor.PopCamera();

		currGameState = GAME_STATE::MENU;
		currGameMenuState = GAME_MENU_TYPE::NEW_GAME;

		ServerPartCleanup();

		eraseWindowContent();
		DrawMenu();
		DrawMenuStars();
	}
	else if (key == ARROW_LEFT_KEY || key == ARROW_RIGHT_KEY)
	{
		if (currGameMenuState == GAME_MENU_TYPE::CUSTOMIZATION)
		{
			for (size_t i = 0; i < modelsManager.GetLoadedModels().size(); ++i)
			{
				modelsManager.GetLoadedModels()[i]->SetRenderMode(false);
			}

			if (key == ARROW_LEFT_KEY && modelsManager.GetCurrentLoadedModelsPtr() > 0) modelsManager.SetCurrentLoadedModelsPtr()--;
			if (key == ARROW_RIGHT_KEY && modelsManager.GetCurrentLoadedModelsPtr() < modelsManager.GetLoadedModels().size() - 1) modelsManager.SetCurrentLoadedModelsPtr()++;

			modelsManager.GetLoadedModels()[modelsManager.GetCurrentLoadedModelsPtr()]->SetRenderMode(true);

			((ASphereCamera*)camActor.GetCurrentCamera())->SetObservedObject(modelsManager.GetLoadedModels()[modelsManager.GetCurrentLoadedModelsPtr()]);
		}
	}
	else if (key == ENTER_KEY)
	{
		if (currGameMenuState == GAME_MENU_TYPE::CUSTOMIZATION)
		{
			modelsManager.SetDefaultModelName(modelsManager.GetPlayerModelsNames()[modelsManager.GetCurrentLoadedModelsPtr()]);

			key = ESC_KEY;
			OnGameKeyCheck();
		}
	}
}

void OnMenuKeyCheck()
{
	if (key == MENU_SCROLL_KEY)
	{
		DrawMenuStars();
		key = NOTHING_KEY;
	}
	else if (key == ENTER_KEY)
	{
		generalMenuClass->CheckAddMenuTypesToStorage();
	}

	key = NOTHING_KEY;
}

void GetKey()
{
	if (_kbhit())
	{
		key = _getch();
	}
}

void OnGame()
{
	std::clock_t start;
	start = std::clock();

	GetNewMousePos();

	for (size_t i = 0; i < actors.size(); ++i)
		actors[i]->Tick();

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	key = NULL;

	Rendering(actors, *camActor.GetCurrentCamera(), serverPartData.isServerCreate || clientPartData.isConnect);
	for (size_t i = 0; i < actors.size(); ++i)
		actors[i]->ImageTick();

	ShowImage(serverPartData.isServerCreate || clientPartData.isConnect);

	tick = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	/*SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
	std::cout << "          ";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
	std::cout << tick;*/
}

bool isBeginPlay = true;
void MenuImpl()
{
	currGameMenuState = GAME_MENU_TYPE::NEW_GAME;
	DrawMenu();
	DrawMenuStars();

	while (true)
	{
		GetKey();

		if (currGameState == GAME_STATE::GAME)
		{
			if (isBeginPlay)
			{
				isBeginPlay = false;
				for (size_t i = 0; i < actors.size(); ++i)
					actors[i]->BeginPlay();
			}

			ApplyServerData();
			ApplyClientData();

			OnGameKeyCheck();
			if (key != ESC_KEY)
				OnGame();
		}
		else if (currGameState == GAME_STATE::MENU)
		{
			if (clientPartData.isClientDataReadyForCleanup)
			{
				clientPartData.ClientDataCleanup();
				clientPartData.isClientDataReadyForCleanup = false;
			}

			isBeginPlay = true;
			OnMenuKeyCheck();
		}
	}
}