#pragma once

#include "Preprocessor.h"
#include "ACameraActor.h"

enum class GAME_STATE : char { GAME, MENU };

GAME_STATE currGameState = GAME_STATE::MENU;
ACamerasActor camActor;
char key = NOTHING_KEY;
double tick;

void eraseWindowContent();

void setWall();

void DrawMenu();

void DrawMenuStars();

void OnGameKeyCheck();

void OnMenuKeyCheck();

void GetKey();

void OnGame();

void MenuImpl();