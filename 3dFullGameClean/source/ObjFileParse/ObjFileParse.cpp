
#define _CRT_SECURE_NO_WARNINGS
#include "ObjFileParse.h"

ObjParseInfo ObjParse(const char* filename, const char* fileNameWithoutExtension)
{
    ObjParseInfo objParseInfo;
    std::string line;

    std::ifstream in(filename);
    if (in.is_open())
    {
        while (std::getline(in, line))
        {
            if (line.substr(0, 2) == "v ") {
                std::istringstream v(line.substr(2));
                COORDS vert;
                double x, y, z;
                v >> x; v >> y; v >> z;
                vert = { (float)x, (float)y, (float)z };
                objParseInfo.vertices.push_back(vert);
            }
            else if (line.substr(0, 3) == "vt ") {
                std::istringstream v(line.substr(3));
                COORDS text;
                float x, y;
                v >> x; v >> y;

                text = { x, y, 0 };
                objParseInfo.textures.push_back(text);
            }
            else if (line.substr(0, 2) == "f ") {
                bool is3dLessObjType = false;
                for (size_t i = 0; i < line.size() - 1; ++i)
                {
                    if (line[i] == '/')
                    {
                        is3dLessObjType = (line[i + 1] == '/' ? true : false);
                        break;
                    }
                }

                int a, b, c;
                int n1, n2, n3;
                int t1, t2, t3;

                const char* chh = line.c_str();

                if (is3dLessObjType)
                    sscanf(chh, "f %i//%i %i//%i %i//%i", &a, &n1, &b, &n2, &c, &n3);
                else
                    sscanf(chh, "f %i/%i/%i %i/%i/%i %i/%i/%i", &a, &t1, &n1, &b, &t2, &n2, &c, &t3, &n3);


                objParseInfo.faceIndex.push_back({ { float(a - 1), float(b - 1), float(c - 1) }, { float(t1 - 1), float(t2 - 1), float(t3 - 1) } });
                objParseInfo.facesTextureNames.push_back(objParseInfo.currentTextureName);
            }
            else if (line.substr(0, 7) == "usemtl ") {
                char materilaName[256];
                const char* chh = line.c_str();
                sscanf(chh, "usemtl %s", materilaName);

                std::string mtlF = ".mtl";
                std::string fName = fileNameWithoutExtension;
                fName += mtlF;

                std::string fullMatName = "newmtl ";
                std::string strMaterialName = materilaName;
                fullMatName += strMaterialName;

                std::string line2;
                std::ifstream in2(fName.c_str());
                if (in2.is_open())
                {
                    bool isMaterialDetected = false;
                    while (std::getline(in2, line2))
                    {
                        if (line2.substr(0, 7 + strMaterialName.size()) == fullMatName.c_str())
                        {
                            isMaterialDetected = true;
                        }
                        else if (line2.substr(0, 7) == "map_Kd " && isMaterialDetected)
                        {
                            char text[256];
                            const char* chh = line2.c_str();
                            sscanf(chh, "map_Kd %s", text);

                            objParseInfo.currentTextureName = "Textures/" + std::string(text);

                            break;
                        }
                    }
                }
            }
        }
    }

    in.close();

    return objParseInfo;
}