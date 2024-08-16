#pragma once
#include "Preprocessor.h"
#include "Light.h"
#include "AActor.h"

class ALightActor : public AActor
{
private:
    UPROPERTY(Light, lightSource)
    Light* lightSource;
    
    UPROPERTY(StaticMesh, staticMesh)
    StaticMesh* staticMesh;

    std::vector<COORDS> lightColors;
    int currentTextureIndex;

public:
    ALightActor(Light* lightSource, StaticMesh* staticMesh, int currentTextureIndex = 0);

    COORDS GetLightColor() const noexcept;

    void BeginPlay() override;

    void Tick() override;
};