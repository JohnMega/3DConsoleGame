
#include "ACameraActor.h"

ACamerasActor::ACamerasActor() : ptr(0)
{ }

void ACamerasActor::AddTrigger(ATriggerActor* trigger)
{
    triggers.push_back(trigger);
}

void ACamerasActor::AddCamera(ACamera* camera) noexcept
{
    cameras.push_back(camera);
}

void ACamerasActor::PopCamera() noexcept
{
    cameras.pop_back();
}

ACamera* ACamerasActor::GetCurrentCamera() noexcept
{
    return cameras[ptr];
}

void ACamerasActor::ChangeCamera() noexcept
{
    ptr = ((ptr == cameras.size() - 1) ? 0 : ptr + 1);
}

void ACamerasActor::BeginPlay()
{ }

void ACamerasActor::Tick()
{
    cameras[ptr]->Move();
}