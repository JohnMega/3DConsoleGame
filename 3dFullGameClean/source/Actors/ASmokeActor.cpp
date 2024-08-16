
#include <format>
#include "ASmokeActor.h"
#include "Model.h"
#include "Circle.h"
#include "GlobalVars.h"
#include "ACameraActor.h"
#include "Maths.h"

extern ACamerasActor camActor;
extern std::vector<AActor*> actors;
extern double tick;

ASmokeActor::ASmokeParticleActor::ASmokeParticleActor(StaticMesh* staticMesh) : staticMesh(staticMesh)
{ }

void ASmokeActor::ASmokeParticleActor::BeginPlay()
{ }

void ASmokeActor::ASmokeParticleActor::Tick()
{
    reinterpret_cast<Circle*>(staticMesh)->SetNormVec({ camActor.GetCurrentCamera()->GetCameraCoord().x, camActor.GetCurrentCamera()->GetCameraCoord().y, camActor.GetCurrentCamera()->GetCameraCoord().z });
}

ASmokeActor::ASmokeActor(COORDS particlesSpawnDot, COORDS endCircleLocalCentreCoord, float rad, int currentColorIndex)
    : particlesSpawnDot(particlesSpawnDot)
    , endCircleLocalCentreCoord(endCircleLocalCentreCoord)
    , rad(rad)
    , currentColorIndex(currentColorIndex)
{ }

void ASmokeActor::BeginPlay()
{
    for (size_t i = 0; i < PARTICLES_COUNT; ++i)
    {
        AddActorToStorage<ASmokeActor::ASmokeParticleActor>(actors, new Circle(particlesSpawnDot, { 1,0,0 }, "Textures/SmokeStackFallback" + std::format("{}", currentColorIndex) + "/SmokeStackFallback" + std::format("{}", currentColorIndex), 0.5f, 5, false, 0.1f));
        particles.push_back(actors.back());
        particlesStartDelayTime.push_back(static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 5)));
        particleStartDelayTimeCounters.push_back(0.0f);
        motionCubicRates.push_back({0,0,0});
        
        COORDS endDot;
        endDot.x = -rad + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (rad + rad)));
        endDot.y = -sqrt(pow(rad, 2) - pow(endDot.x, 2)) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * sqrt(pow(rad, 2) - pow(endDot.x, 2)))));
        endDot.z = 0;
        particlesEndDot.push_back(endDot + endCircleLocalCentreCoord + particlesSpawnDot);
    }
}

void ASmokeActor::ParticleSpawnHandle()
{
    for (size_t i = 0; i < PARTICLES_COUNT; ++i)
    {
        if (((Circle*)particles[i]->GetStaticMesh())->IsCircleRender() == false)
        {
            particleStartDelayTimeCounters[i] += tick;

            if (particleStartDelayTimeCounters[i] >= particlesStartDelayTime[i])
            {
                ((Circle*)particles[i]->GetStaticMesh())->SetRenderMode(true);
                particleStartDelayTimeCounters[i] = 0;
            }
        }
    }
}

void ASmokeActor::Tick()
{
    ParticleSpawnHandle();

    for (size_t i = 0; i < PARTICLES_COUNT; ++i)
    {
        if (particles[i]->GetStaticMesh()->GetCentreCoord().x >= particlesEndDot[i].x - 0.1f && particles[i]->GetStaticMesh()->GetCentreCoord().x <= particlesEndDot[i].x + 0.1f
            && particles[i]->GetStaticMesh()->GetCentreCoord().y >= particlesEndDot[i].y - 0.1f && particles[i]->GetStaticMesh()->GetCentreCoord().y <= particlesEndDot[i].y + 0.1f
            && particles[i]->GetStaticMesh()->GetCentreCoord().z >= particlesEndDot[i].z - 0.1f && particles[i]->GetStaticMesh()->GetCentreCoord().z <= particlesEndDot[i].z + 0.1f)
        {
            ((Circle*)particles[i]->GetStaticMesh())->AnimationHandle();

            if (((Circle*)particles[i]->GetStaticMesh())->GetSpriteCurrentFrame() == ((Circle*)particles[i]->GetStaticMesh())->GetSpriteFrameCount() - 1)
            {
                ((Circle*)particles[i]->GetStaticMesh())->GetSpriteCurrentFrame() = 0;
                ((Circle*)particles[i]->GetStaticMesh())->SetRenderMode(false);
                particles[i]->GetStaticMesh()->SetCentreCoord() = particlesSpawnDot;
                motionCubicRates[i] = { 0,0,0 };
            }
        }

        if (((Circle*)particles[i]->GetStaticMesh())->IsCircleRender() == true)
        {
            COORDS startSphereCoord = ToSphereFromCartesianCoords(particles[i]->GetStaticMesh()->GetCentreCoord());
            COORDS endSphereCoord = ToSphereFromCartesianCoords(particlesEndDot[i]);

            MotionCubic(endSphereCoord.x, tick / (float)2, &startSphereCoord.x, &motionCubicRates[i].x);
            MotionCubic(endSphereCoord.y, tick / (float)2, &startSphereCoord.y, &motionCubicRates[i].y);
            MotionCubic(endSphereCoord.z, tick / (float)2, &startSphereCoord.z, &motionCubicRates[i].z);

            particles[i]->GetStaticMesh()->SetCentreCoord() = ToCartesianFromSphereCoords(startSphereCoord);
        }
    }
}