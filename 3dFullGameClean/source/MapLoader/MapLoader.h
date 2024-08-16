#pragma once

#include <format>

#include "GlobalVars.h"
#include "ObjFileParse.h"
#include "Triangle.h"
#include "InvisibleParallelepiped.h"
#include "ACollisionActor.h"
#include "ACharacterCamera.h"
#include "SkyboxPart.h"
#include "ASkyboxPart.h"
#include "ACubemapActor.h"
#include "AEnvFade.h"
#include "ACameraShaker.h"
#include "Circle.h"
#include "FantomModel.h"
#include "ModelsManager.h"

extern std::vector<class AActor*> actors;
extern ACamerasActor camActor;
extern ModelsManager modelsManager;

#define DEFINE_FUNCTION(Name) void Name(Stack& stack)

#define INCLUDE_FUNCTION(FuncName) funcTable.push_back(FuncName)

std::vector<void (*)(class Stack&)> funcTable;

DEFINE_FUNCTION(ExPar);
DEFINE_FUNCTION(ExPyramid);
DEFINE_FUNCTION(ExLight);
DEFINE_FUNCTION(ExPlayer);
DEFINE_FUNCTION(ExTriangle);
DEFINE_FUNCTION(ExModel);
DEFINE_FUNCTION(ExSkybox);
DEFINE_FUNCTION(ExFire);
DEFINE_FUNCTION(ExCircle);
DEFINE_FUNCTION(ExSmoke);
DEFINE_FUNCTION(ExMovementPart);
DEFINE_FUNCTION(ExCubemap);
DEFINE_FUNCTION(ExClipWall);
DEFINE_FUNCTION(ExTrigger);
DEFINE_FUNCTION(ExAreaPortal);
DEFINE_FUNCTION(ExEnvShake);
DEFINE_FUNCTION(ExSkyCamera);
DEFINE_FUNCTION(ExVolumeSkybox);
DEFINE_FUNCTION(ExEnvFade);

std::string mapName;

class Stack
{
    static inline bool isFuncTableEnable = false;

private:
    std::string code;

public:
    char* codePtr;

public:
    Stack()
    {
        if (!isFuncTableEnable)
        {
            isFuncTableEnable = true;
            INCLUDE_FUNCTION(ExPar);
            INCLUDE_FUNCTION(ExPyramid);
            INCLUDE_FUNCTION(ExLight);
            INCLUDE_FUNCTION(ExPlayer);
            INCLUDE_FUNCTION(ExTriangle);
            INCLUDE_FUNCTION(ExModel);
            INCLUDE_FUNCTION(ExSkybox);
            INCLUDE_FUNCTION(ExFire);
            INCLUDE_FUNCTION(ExCircle);
            INCLUDE_FUNCTION(ExSmoke);
            INCLUDE_FUNCTION(ExMovementPart);
            INCLUDE_FUNCTION(ExCubemap);
            INCLUDE_FUNCTION(ExClipWall);
            INCLUDE_FUNCTION(ExTrigger);
            INCLUDE_FUNCTION(ExAreaPortal);
            INCLUDE_FUNCTION(ExEnvShake);
            INCLUDE_FUNCTION(ExSkyCamera);
            INCLUDE_FUNCTION(ExVolumeSkybox);
            INCLUDE_FUNCTION(ExEnvFade);
        }

        std::string line;
        std::ifstream in(mapName);
        if (in.is_open())
        {
            while (std::getline(in, line))
            {
                code.append(line);
            }
        }

        codePtr = const_cast<char*>(code.c_str());
    }

    std::string GetCode()
    {
        return code;
    }

    char* GetCodePtr()
    {
        return codePtr;
    }

    void Step()
    {
        if (*codePtr == '|')
        {
            // Add player model
            FantomModel* model = modelsManager.CreateDefaultPlayerFantomModel();
            AddActorToStorage<ACharacterActor>(actors, model, &camActor);

            return;
        }

        int index = atoi(codePtr); while (*codePtr++ != ':') {}
        codePtr--;
        funcTable[index](*this);
    }
};

void LoadMap()
{
    Stack stack;

    stack.Step();
}

DEFINE_FUNCTION(ExTriangle) {}
DEFINE_FUNCTION(ExMovementPart) {}
DEFINE_FUNCTION(ExCubemap) {}

DEFINE_FUNCTION(ExPar)
{
    stack.codePtr++;
    float length = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float width = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float height = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    bool IsRotateAroundZAxis = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    bool IsRotateAroundXAxis = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    bool IsRotateAroundYAxis = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float angleRotationSum = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    int color = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

    stack.codePtr++;
    Range parRange;
    parRange.xStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.xEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.yStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.yEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.zStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.zEnd = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    // Parab mode params
    stack.codePtr++;
    bool isParab = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    stack.codePtr++;
    Range parabOwnerRange;
    parabOwnerRange.xStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parabOwnerRange.xEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parabOwnerRange.yStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parabOwnerRange.yEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parabOwnerRange.zStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parabOwnerRange.zEnd = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr += 2;
    COORDS centreCoords;

    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr++;

    char fileName[256]; int counter = 0;
    std::string textureName;
    while (*stack.codePtr != ';') fileName[counter++] = *stack.codePtr++;
    fileName[counter] = NULL;
    stack.codePtr++;
    textureName = fileName;

    float widthTextureCount = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float heightTextureCount = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    bool isHasCollision = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}
    
    stack.codePtr++;
    COORDS cubemapCentreCoords;
    cubemapCentreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    cubemapCentreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    cubemapCentreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    Parallelepiped* newObj = new Parallelepiped(length, width, height, centreCoords, IsRotateAroundZAxis, IsRotateAroundXAxis, IsRotateAroundYAxis, color);
    if (newObj->IsRotate()) newObj->Rotation(angleRotationSum);

    *reinterpret_cast<bool*>(sizeof(StaticMesh) + sizeof(Collision) + 24 + (char*)newObj) = isParab;
    *reinterpret_cast<Range*>(sizeof(StaticMesh) + sizeof(Collision) + 28 + (char*)newObj) = parabOwnerRange;
    newObj->SetRange() = parRange;
    
    if (textureName != "NULL")
    {
        newObj->SetObjTextureMode(true);
        newObj->SetObjTextureParams(textureName, widthTextureCount, heightTextureCount);
    }
    
    if(isHasCollision)
        AddActorToStorage<ACollisionActor>(actors, newObj, newObj);
    else
        AddActorToStorage<ABaseActor>(actors, newObj);
    
    AActor* currLoadedActor = actors.back();
    
    if (!(cubemapCentreCoords.x == 0 && cubemapCentreCoords.y == 0 && cubemapCentreCoords.z == 0))
    {
        AddActorToStorage<ACubemapActor>(actors, actors.back());
    }
    
    stack.codePtr += 3;
    while (*stack.codePtr != '|')
    {
        COORDS triggerCentreCoord;
        int typeOfTrigger;
        triggerCentreCoord.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        triggerCentreCoord.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        triggerCentreCoord.z = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        typeOfTrigger = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}
        
        AActor* currentTrigger = nullptr;
        for (size_t i = 0; i < actors.size(); ++i)
        {
            if (actors[i]->isActorHasStaticMesh())
                if (actors[i]->GetStaticMesh()->GetCentreCoord() == triggerCentreCoord)
                {
                    currentTrigger = actors[i];
                    break;
                }
        }
        
        currLoadedActor->AddFuncToTrigger(currentTrigger, typeOfTrigger, ATriggerActor::ReadInfoFuncTable[typeOfTrigger](stack.codePtr));
        
        stack.codePtr += 2;
    }

    stack.codePtr++;
    stack.Step();
}

DEFINE_FUNCTION(ExSkyCamera)
{
    stack.codePtr += 2;
    COORDS centreCoords;

    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    InvisibleParallelepiped* newObj = new InvisibleParallelepiped(1, 1, 1, centreCoords, false, false, false, 1, objectType::SKY_CAMERA);

    AddActorToStorage<ABaseActor>(actors, newObj);

    stack.Step();
}

DEFINE_FUNCTION(ExVolumeSkybox)
{
    stack.codePtr++;
    float length = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float width = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float height = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    bool IsRotateAroundZAxis = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    bool IsRotateAroundXAxis = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    bool IsRotateAroundYAxis = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float angleRotationSum = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    stack.codePtr++;
    Range parRange;
    parRange.xStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.xEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.yStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.yEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.zStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.zEnd = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr += 2;
    COORDS centreCoords;

    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    InvisibleParallelepiped* newObj = new InvisibleParallelepiped(length, width, height, centreCoords, false, false, false, 1, objectType::VOLUME_SKYBOX);

    *reinterpret_cast<bool*>(sizeof(StaticMesh) + 24 + (char*)newObj) = false;
    newObj->SetRange() = parRange;

    AddActorToStorage<ACollisionActor>(actors, newObj, newObj);

    stack.Step();
}

DEFINE_FUNCTION(ExTrigger)
{
    stack.codePtr++;
    float length = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float width = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float height = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    stack.codePtr++;
    Range parRange;
    parRange.xStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.xEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.yStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.yEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.zStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.zEnd = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr += 2;
    COORDS centreCoords;

    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}
    
    stack.codePtr += 2;
    COORDS observedObjCentreCoord;

    observedObjCentreCoord.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    observedObjCentreCoord.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    observedObjCentreCoord.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    InvisibleParallelepiped* newObj = new InvisibleParallelepiped(length, width, height, centreCoords, false, false, false, 1, objectType::TRIGGER);
    AddActorToStorage<ATriggerActor>(actors, newObj, observedObjCentreCoord, parRange);
    
    stack.Step();
}

DEFINE_FUNCTION(ExAreaPortal)
{
    stack.codePtr++;
    float length = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float width = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float height = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    stack.codePtr++;
    Range parRange;
    parRange.xStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.xEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.yStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.yEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.zStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.zEnd = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr += 2;
    COORDS centreCoords;

    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    InvisibleParallelepiped* newObj = new InvisibleParallelepiped(length, width, height, centreCoords, false, false, false, 1, objectType::AREA_PORTAL);

    newObj->SetRange() = parRange;
    AddActorToStorage<ABaseActor>(actors, newObj);

    stack.Step();
}

DEFINE_FUNCTION(ExClipWall)
{
    stack.codePtr++;
    float length = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float width = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float height = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    stack.codePtr++;
    Range parRange;
    parRange.xStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.xEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.yStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.yEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.zStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.zEnd = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr += 2;
    COORDS centreCoords;

    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    InvisibleParallelepiped* newObj = new InvisibleParallelepiped(length, width, height, centreCoords, false, false, false, 1, objectType::CLIP_WALL);

    newObj->SetRange() = parRange;

    AddActorToStorage<ACollisionActor>(actors, newObj, newObj);
    AActor* currLoadedActor = actors.back();

    stack.codePtr += 3;
    while (*stack.codePtr != '|')
    {
        COORDS triggerCentreCoord;
        int typeOfTrigger;
        triggerCentreCoord.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        triggerCentreCoord.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        triggerCentreCoord.z = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        typeOfTrigger = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

        AActor* currentTrigger = nullptr;
        for (size_t i = 0; i < actors.size(); ++i)
        {
            if (actors[i]->isActorHasStaticMesh())
                if (actors[i]->GetStaticMesh()->GetCentreCoord() == triggerCentreCoord)
                {
                    currentTrigger = actors[i];
                    break;
                }
        }

        currLoadedActor->AddFuncToTrigger(currentTrigger, typeOfTrigger, ATriggerActor::ReadInfoFuncTable[typeOfTrigger](stack.codePtr));

        stack.codePtr += 2;
    }

    stack.codePtr++;
    stack.Step();
}

DEFINE_FUNCTION(ExFire)
{
    stack.codePtr += 2;
    COORDS centreCoords;

    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr += 2;
    COORDS cubemapCentreCoords;
    cubemapCentreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    cubemapCentreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    cubemapCentreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    Circle* newObj = new Circle(centreCoords, { 1,0,0 }, "Textures/env_fire/env_fire", 3, 5);

    AddActorToStorage<ABaseActor>(actors, newObj);

    if (!(cubemapCentreCoords.x == 0 && cubemapCentreCoords.y == 0 && cubemapCentreCoords.z == 0))
    {
        AddActorToStorage<ACubemapActor>(actors, actors.back());
    }

    stack.Step();
}

DEFINE_FUNCTION(ExEnvShake)
{
    stack.codePtr++;
    float shakePower;
    shakePower = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    stack.codePtr++;
    COORDS centreCoords;

    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    AddActorToStorage<ACameraShaker>(actors, shakePower);
    AActor* currLoadedActor = actors.back();

    stack.codePtr += 3;
    while (*stack.codePtr != '|')
    {
        COORDS triggerCentreCoord;
        int typeOfTrigger;
        triggerCentreCoord.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        triggerCentreCoord.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        triggerCentreCoord.z = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        typeOfTrigger = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

        AActor* currentTrigger = nullptr;
        for (size_t i = 0; i < actors.size(); ++i)
        {
            if (actors[i]->isActorHasStaticMesh())
                if (actors[i]->GetStaticMesh()->GetCentreCoord() == triggerCentreCoord)
                {
                    currentTrigger = actors[i];
                    break;
                }
        }

        currLoadedActor->AddFuncToTrigger(currentTrigger, typeOfTrigger, ATriggerActor::ReadInfoFuncTable[typeOfTrigger](stack.codePtr));

        stack.codePtr += 2;
    }

    stack.codePtr++;
    stack.Step();
}

DEFINE_FUNCTION(ExEnvFade)
{
    stack.codePtr += 2;
    COORDS centreCoords;

    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    AddActorToStorage<AEnvFade>(actors);
    AActor* currLoadedActor = actors.back();
    
    stack.codePtr += 3;
    while (*stack.codePtr != '|')
    {
        COORDS triggerCentreCoord;
        int typeOfTrigger;
        triggerCentreCoord.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        triggerCentreCoord.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        triggerCentreCoord.z = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        typeOfTrigger = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

        AActor* currentTrigger = nullptr;
        for (size_t i = 0; i < actors.size(); ++i)
        {
            if (actors[i]->isActorHasStaticMesh())
                if (actors[i]->GetStaticMesh()->GetCentreCoord() == triggerCentreCoord)
                {
                    currentTrigger = actors[i];
                    break;
                }
        }

        currLoadedActor->AddFuncToTrigger(currentTrigger, typeOfTrigger, ATriggerActor::ReadInfoFuncTable[typeOfTrigger](stack.codePtr));

        stack.codePtr += 2;
    }

    stack.codePtr++;
    stack.Step();
}

DEFINE_FUNCTION(ExSmoke)
{
    stack.codePtr += 2;
    COORDS particlesSpawnDot;

    particlesSpawnDot.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    particlesSpawnDot.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    particlesSpawnDot.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr += 2;
    COORDS endCircleCentreCoords;

    endCircleCentreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    endCircleCentreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    endCircleCentreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr++;
    float rad;
    rad = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    int currentColorIndex;
    currentColorIndex = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

    AddActorToStorage<ASmokeActor>(actors, particlesSpawnDot, endCircleCentreCoords, rad, currentColorIndex);

    stack.Step();
}

DEFINE_FUNCTION(ExCircle)
{
    stack.codePtr++;
    float rad = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    stack.codePtr++;
    COORDS centreCoords;

    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr++;

    char fileName[256]; int counter = 0;
    std::string textureName;
    while (*stack.codePtr != ';') fileName[counter++] = *stack.codePtr++;
    fileName[counter] = NULL;
    stack.codePtr++;
    textureName = fileName;

    stack.codePtr++;
    COORDS normVec;

    normVec.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    normVec.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    normVec.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}
    stack.codePtr++;

    bool circleRotateMode = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}
    bool circleLimpidMode = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

    stack.codePtr++;
    COORDS cubemapCentreCoords;
    cubemapCentreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    cubemapCentreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    cubemapCentreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    Circle* newObj = new Circle(centreCoords, normVec, textureName, rad, 5);
    newObj->SetCircleRotateMode(circleRotateMode);
    newObj->SetLimpidMode(circleLimpidMode);

    AddActorToStorage<ABaseActor>(actors, newObj);

    if (!(cubemapCentreCoords.x == 0 && cubemapCentreCoords.y == 0 && cubemapCentreCoords.z == 0))
    {
        AddActorToStorage<ACubemapActor>(actors, actors.back());
    }

    stack.Step();
}

DEFINE_FUNCTION(ExSkybox)
{
    stack.codePtr++;
    float length = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float width = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float height = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    int color = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

    stack.codePtr++;
    Range parRange;
    parRange.xStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.xEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.yStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.yEnd = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.zStart = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    parRange.zEnd = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr += 2;
    COORDS centreCoords;

    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr++;

    char fileName[256]; int counter = 0;
    std::string textureName;
    while (*stack.codePtr != ';') fileName[counter++] = *stack.codePtr++;
    fileName[counter] = NULL;
    stack.codePtr++;
    textureName = fileName;

    int numberOfSkyboxPart;
    numberOfSkyboxPart = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

    SkyboxPart* newObj = new SkyboxPart(length, width, height, centreCoords, color);
    newObj->SetRange() = parRange;

    if (textureName != "NULL")
    {
        newObj->SetObjTextureMode(true);
        newObj->SetObjTextureParams(textureName);
    }

    AddActorToStorage<ASkyboxPart>(actors, newObj, numberOfSkyboxPart);

    stack.Step();
}

DEFINE_FUNCTION(ExPyramid)
{
    stack.codePtr++;
    float lowerBaselength = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float lowerBasewidth = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float height = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float upperBaseLength = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    float upperBaseWidth = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    int color = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

    stack.codePtr++;
    COORDS centreCoords;

    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr++;
    COORDS cubemapCentreCoords;
    cubemapCentreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    cubemapCentreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    cubemapCentreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    Pyramid* newObj = new Pyramid(lowerBaselength, lowerBasewidth, height, upperBaseLength, upperBaseWidth, centreCoords, color);

    AddActorToStorage<ACollisionActor>(actors, newObj, newObj);
    AActor* currLoadedActor = actors.back();

    if (!(cubemapCentreCoords.x == 0 && cubemapCentreCoords.y == 0 && cubemapCentreCoords.z == 0))
    {
        AddActorToStorage<ACubemapActor>(actors, actors.back());
    }

    stack.codePtr += 3;
    while (*stack.codePtr != '|')
    {
        COORDS triggerCentreCoord;
        int typeOfTrigger;
        triggerCentreCoord.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        triggerCentreCoord.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        triggerCentreCoord.z = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        typeOfTrigger = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

        AActor* currentTrigger = nullptr;
        for (size_t i = 0; i < actors.size(); ++i)
        {
            if (actors[i]->isActorHasStaticMesh())
                if (actors[i]->GetStaticMesh()->GetCentreCoord() == triggerCentreCoord)
                {
                    currentTrigger = actors[i];
                    break;
                }
        }

        currLoadedActor->AddFuncToTrigger(currentTrigger, typeOfTrigger, ATriggerActor::ReadInfoFuncTable[typeOfTrigger](stack.codePtr));

        stack.codePtr += 2;
    }

    stack.codePtr++;
    stack.Step();
}

DEFINE_FUNCTION(ExLight)
{
    // Light
    stack.codePtr++;
    int IValue = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

    COORDS lightCentreCoords;

    stack.codePtr++;
    lightCentreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    lightCentreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    lightCentreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr++;
    int currentLightTextureIndex = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

    Light* light = new Light(IValue, lightCentreCoords);
    AddActorToStorage<ALightActor>(actors, light, new Circle(lightCentreCoords, { 1,0,0 }, "Textures/env_glow/env_glow" + std::format("{}.bmp", currentLightTextureIndex), 1, 5), currentLightTextureIndex);

    stack.Step();
}

DEFINE_FUNCTION(ExPlayer)
{
    stack.codePtr += 2;
    COORDS centreCoords;

    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    camActor.AddCamera(new ACharacterCamera(centreCoords));
    AddActorToStorage(actors, &camActor);

    stack.Step();
}

void LoadStaticModel(std::string modelFileName, std::string objFileNameWithExtensions, Model* model, bool& isModelAnimated);

DEFINE_FUNCTION(ExModel)
{
    stack.codePtr++;
    
    std::string modelFileName, objFileNameWithExtensions = "Models/";
    char fileName[256]; int counter = 0;
    while (*stack.codePtr != ';') fileName[counter++] = *stack.codePtr++;
    fileName[counter] = NULL;
    stack.codePtr++;
    modelFileName = fileName;
    objFileNameWithExtensions += modelFileName;
    
    stack.codePtr++;
    COORDS centreCoords;
    centreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    centreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    stack.codePtr++;
    float startPolygonDist;
    startPolygonDist = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    float scale;
    scale = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}

    AActor* currLoadedActor = nullptr;
    Model* model = new Model;
    model->AddNewModelSlot();
    bool isModelAnimated;
    std::ifstream in(FILE_EXTENSION(std::string(objFileNameWithExtensions), EXTENSION_CAST(obj)));
    if (in.is_open())
    {
        LoadStaticModel(modelFileName, objFileNameWithExtensions, model, isModelAnimated);
        currLoadedActor = actors.back();
    }
    else
    {
        std::string modelAnimFrame = objFileNameWithExtensions + std::string("0000");
        std::ifstream in(FILE_EXTENSION(std::string(modelAnimFrame), EXTENSION_CAST(obj)));
        if (!in.is_open())
        {
            LoadStaticModel("Error", "Models/Error/Error", model, isModelAnimated);
            currLoadedActor = actors.back();

            stack.codePtr++;
            while (*stack.codePtr != '}')
            {
                char rotationAxis = *stack.codePtr++;
                float angle = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
                model->Rotation(angle, rotationAxis);
            }

            model->SetCentreCoord() = centreCoords;
            model->UpdateRange();

            stack.codePtr++;

            stack.codePtr += 2;
            COORDS cubemapCentreCoords;
            cubemapCentreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
            cubemapCentreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
            cubemapCentreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

            if (!(cubemapCentreCoords.x == 0 && cubemapCentreCoords.y == 0 && cubemapCentreCoords.z == 0))
            {
                AddActorToStorage<ACubemapActor>(actors, actors.back());
            }

            stack.codePtr += 3;
            while (*stack.codePtr != '|')
            {
                COORDS triggerCentreCoord;
                int typeOfTrigger;
                triggerCentreCoord.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
                triggerCentreCoord.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
                triggerCentreCoord.z = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
                typeOfTrigger = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

                AActor* currentTrigger = nullptr;
                for (size_t i = 0; i < actors.size(); ++i)
                {
                    if (actors[i]->isActorHasStaticMesh())
                        if (actors[i]->GetStaticMesh()->GetCentreCoord() == triggerCentreCoord)
                        {
                            currentTrigger = actors[i];
                            break;
                        }
                }

                currLoadedActor->AddFuncToTrigger(currentTrigger, typeOfTrigger, ATriggerActor::ReadInfoFuncTable[typeOfTrigger](stack.codePtr));

                stack.codePtr += 2;
            }

            model->SetScale(scale);

            stack.codePtr++;
            stack.Step();
            return;
        }

        isModelAnimated = true;
    }

    if (isModelAnimated)
    {
        model->SetAnimationInfo().isAnimated = true;

        while (true)
        {
            std::string modelAnimFrame = objFileNameWithExtensions + std::format("{:04}", model->GetAnimationInfo().frameCounter);

            std::ifstream in(FILE_EXTENSION(std::string(modelAnimFrame), EXTENSION_CAST(obj)));
            if (!in.is_open())
            {
                model->UpdateRange();
                model->SetFileName() = fileName;
                AddActorToStorage<ABaseActor>(actors, model);
                currLoadedActor = actors.back();
                break;
            }

            model->SetAnimationInfo().frameCounter++;
            ObjParseInfo objParseInfo = ObjParse(FILE_EXTENSION(std::string(modelAnimFrame), EXTENSION_CAST(obj)).c_str(), modelAnimFrame.c_str());

            for (size_t i = 0; i < objParseInfo.faceIndex.size(); ++i)
            {
                if (model->GetAnimationInfo().frameCounter > 1)
                    model->GetAnimModelPolygons().emplace_back();

                model->GetAnimModelPolygons()[model->GetAnimationInfo().frameCounter - 1].push_back(new Triangle(objParseInfo.vertices[objParseInfo.faceIndex[i][0].x], objParseInfo.vertices[objParseInfo.faceIndex[i][0].y], objParseInfo.vertices[objParseInfo.faceIndex[i][0].z], model, objParseInfo.faceIndex[i][1]));
                ((Triangle*)model->GetAnimModelPolygons()[model->GetAnimationInfo().frameCounter - 1].back())->SetTextureParams(objParseInfo.facesTextureNames[i], objParseInfo.textures[objParseInfo.faceIndex[i][1].x], objParseInfo.textures[objParseInfo.faceIndex[i][1].y], objParseInfo.textures[objParseInfo.faceIndex[i][1].z]);
            }

            size_t verticesSize = objParseInfo.vertices.size();
            size_t facesSize = objParseInfo.faceIndex.size();
            size_t texturesSize = objParseInfo.textures.size();
            size_t texturesNamesSize = objParseInfo.facesTextureNames.size();
            for (size_t i = 0; i < verticesSize; ++i) objParseInfo.vertices.pop_back();
            for (size_t i = 0; i < facesSize; ++i) objParseInfo.faceIndex.pop_back();
            for (size_t i = 0; i < texturesSize; ++i) objParseInfo.textures.pop_back();
            for (size_t i = 0; i < texturesNamesSize; ++i) objParseInfo.facesTextureNames.pop_back();
        }
    }

    stack.codePtr++;
    while (*stack.codePtr != '}')
    {
        char rotationAxis = *stack.codePtr++;
        float angle = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        model->Rotation(angle, rotationAxis);
    }

    model->SetCentreCoord() = centreCoords;
    model->UpdateRange();

    stack.codePtr++;
    
    stack.codePtr += 2;
    COORDS cubemapCentreCoords;
    cubemapCentreCoords.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    cubemapCentreCoords.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
    cubemapCentreCoords.z = atof(stack.codePtr); while (*stack.codePtr++ != '}') {}

    if (!(cubemapCentreCoords.x == 0 && cubemapCentreCoords.y == 0 && cubemapCentreCoords.z == 0))
    {
        AddActorToStorage<ACubemapActor>(actors, actors.back());
    }

    stack.codePtr += 3;
    while (*stack.codePtr != '|')
    {
        COORDS triggerCentreCoord;
        int typeOfTrigger;
        triggerCentreCoord.x = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        triggerCentreCoord.y = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        triggerCentreCoord.z = atof(stack.codePtr); while (*stack.codePtr++ != ';') {}
        typeOfTrigger = atoi(stack.codePtr); while (*stack.codePtr++ != ';') {}

        AActor* currentTrigger = nullptr;
        for (size_t i = 0; i < actors.size(); ++i)
        {
            if (actors[i]->isActorHasStaticMesh())
                if (actors[i]->GetStaticMesh()->GetCentreCoord() == triggerCentreCoord)
                {
                    currentTrigger = actors[i];
                    break;
                }
        }

        currLoadedActor->AddFuncToTrigger(currentTrigger, typeOfTrigger, ATriggerActor::ReadInfoFuncTable[typeOfTrigger](stack.codePtr));

        stack.codePtr += 2;
    }

    model->SetScale(scale);

    stack.codePtr++;
    stack.Step();
}

void LoadStaticModel(std::string modelFileName, std::string objFileNameWithExtensions, Model* model, bool& isModelAnimated)
{
    ObjParseInfo objParseInfo = ObjParse(FILE_EXTENSION(std::string(objFileNameWithExtensions), EXTENSION_CAST(obj)).c_str(), objFileNameWithExtensions.c_str());
    isModelAnimated = false;

    for (size_t i = 0; i < objParseInfo.faceIndex.size(); ++i)
    {
        model->GetModelPolygons().push_back(new Triangle(objParseInfo.vertices[objParseInfo.faceIndex[i][0].x], objParseInfo.vertices[objParseInfo.faceIndex[i][0].y], objParseInfo.vertices[objParseInfo.faceIndex[i][0].z], model, objParseInfo.faceIndex[i][1]));
        ((Triangle*)model->GetModelPolygons().back())->SetTextureParams(objParseInfo.facesTextureNames[i], objParseInfo.textures[objParseInfo.faceIndex[i][1].x], objParseInfo.textures[objParseInfo.faceIndex[i][1].y], objParseInfo.textures[objParseInfo.faceIndex[i][1].z]);
    }

    model->UpdateRange();
    model->SetFileName() = modelFileName;
    AddActorToStorage<ABaseActor>(actors, model);

    size_t verticesSize = objParseInfo.vertices.size();
    size_t facesSize = objParseInfo.faceIndex.size();
    size_t texturesSize = objParseInfo.textures.size();
    size_t texturesNamesSize = objParseInfo.facesTextureNames.size();
    for (size_t i = 0; i < verticesSize; ++i) objParseInfo.vertices.pop_back();
    for (size_t i = 0; i < facesSize; ++i) objParseInfo.faceIndex.pop_back();
    for (size_t i = 0; i < texturesSize; ++i) objParseInfo.textures.pop_back();
    for (size_t i = 0; i < texturesNamesSize; ++i) objParseInfo.facesTextureNames.pop_back();
}