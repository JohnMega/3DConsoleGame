#pragma once

#include "Preprocessor.h"
#include "AActor.h"
#include "Model.h"
#include "ABaseActor.h"

class ASmokeActor : public AActor
{
    class ASmokeParticleActor : public AActor
    {
    private:
        UPROPERTY(StaticMesh, staticMesh)
        StaticMesh* staticMesh;

        float currentRotateAngle = 0;

    public:
        ASmokeParticleActor(StaticMesh* staticMesh);

        void BeginPlay() override;

        void Tick() override;
    };

    static const int PARTICLES_COUNT = 100;

private:
    COORDS particlesSpawnDot;
    std::vector<AActor*> particles;
    std::vector<float> particlesStartDelayTime;
    std::vector<float> particleStartDelayTimeCounters;
    std::vector<COORDS> particlesEndDot;

    std::vector<COORDS> motionCubicRates;

    float rad;
    int currentColorIndex;
    COORDS endCircleLocalCentreCoord;

private:
    void ParticleSpawnHandle();

public:
    ASmokeActor(COORDS particlesSpawnDot, COORDS endCircleLocalCentreCoord, float rad, int currentColorIndex);

    void BeginPlay() override;

    void Tick() override;
};