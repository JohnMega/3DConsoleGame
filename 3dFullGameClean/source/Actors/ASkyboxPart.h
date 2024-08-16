#pragma once
#include "Preprocessor.h"
#include "AActor.h"

class ASkyboxPart : public AActor
{
private:
    UPROPERTY(StaticMesh, staticMesh)
    StaticMesh* staticMesh;

    int numbOfPart;

public:
    static const int FORWARD_PART = 0;
    static const int BACK_PART = 2;
    static const int LEFT_PART = 3;
    static const int RIGHT_PART = 1;
    static const int UP_PART = 4;
    static const int DOWN_PART = 5;

public:
    ASkyboxPart(StaticMesh* staticMesh, int numberOfSkyboxPart);

    int GetNumberOfSkyboxPart() const noexcept;

    void BeginPlay() override;

    void Tick() override;
};