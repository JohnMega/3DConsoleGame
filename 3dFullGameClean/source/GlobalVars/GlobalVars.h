#pragma once

#include <vector>
#include "Maths.h"

enum class objectType : char { PARALLELEPIPED, PYRAMID, LIGHT, PLAYER, TRIANGLE, MODEL, SKYBOX, ENV_FIRE, CIRCLE, ENV_PARTICLES, MOVEMENT_PART
    , ENV_CUBEMAP, CLIP_WALL, TRIGGER, AREA_PORTAL, ENV_SHAKE, SKY_CAMERA, VOLUME_SKYBOX, ENV_FADE };
enum class EDITOR_STATE : char { GAME, EDITOR, SUB_EDITOR };

const int VK_FORWARD_KEY = 0x57;
const int VK_BACK_KEY = 0x53;
const int VK_LEFT_KEY = 0x41;
const int VK_RIGHT_KEY = 0x44;

const int SCREEN_WIDTH = 32 * 2;
const int SCREEN_HEIGHT = 28;

const COORDS RGBblue = { 59, 60, 158 };
const COORDS RGBgreen = { 59, 158, 59 };
const COORDS RGBcyan = { 58, 159, 163 };
const COORDS RGBred = { 163, 58, 58 };
const COORDS RGBpurple = { 146, 58, 163 };
const COORDS RGByellow = { 163, 155, 58 };
const COORDS RGBwhite = { 217, 217, 217 };
const COORDS RGBgray = { 167, 167, 167 };
const COORDS RGBBrightBlue = { 0, 0, 255 };
const COORDS RGBBrightGreen = { 0, 255, 0 };
const COORDS RGBBrightCyan = { 0, 255, 230 };
const COORDS RGBBrightRed = { 255, 0, 0 };
const COORDS RGBBrightPurple = { 233, 0, 255 };
const COORDS RGBBrightYellow = { 251, 255, 150 };
const COORDS RGBBrightWhite = { 255, 255, 255 };

struct
{
    std::vector<COORDS> GeneralRGBConsoleColors = { RGBblue, RGBgreen, RGBcyan, RGBred, RGBpurple, RGByellow, RGBwhite, RGBgray
    , RGBBrightBlue, RGBBrightGreen, RGBBrightCyan, RGBBrightRed, RGBBrightPurple, RGBBrightYellow, RGBBrightWhite };

} ConsoleRBGColors;

template <typename Type, typename... Args>
void AddActorToStorage(std::vector<class AActor*>& actors, const Args&... args)
{
    actors.push_back(new Type(args...));
}

template <typename Type>
void AddActorToStorage(std::vector<AActor*>& actors, Type* actor)
{
    actors.push_back(actor);
}