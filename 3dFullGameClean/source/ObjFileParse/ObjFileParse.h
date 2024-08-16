#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Maths.h"
#include "GlobalVars.h"

extern char key;

struct ObjParseInfo
{
    std::vector<COORDS> vertices;
    std::vector<COORDS> textures;
    std::vector<std::vector<COORDS>> faceIndex;
    std::vector<std::string> facesTextureNames;
    std::string currentTextureName;
};

ObjParseInfo ObjParse(const char* filename, const char* fileNameWithoutExtension);