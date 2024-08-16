#pragma once
#include "Preprocessor.h"
#include "AActor.h"
#include "Model.h"
#include "Cubemap.h"

class ACubemapActor : public AActor
{
private:
    AActor* observedObj;

public:
    ACubemapActor(AActor* observedObj);

    void BeginPlay() override;

    void Tick() override;
};