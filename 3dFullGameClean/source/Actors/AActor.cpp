
#include "AActor.h"

void AActor::ImageTick()
{ }

float& AActor::GetCurrentRotateAngle() noexcept
{
    float temp = 0;
    return temp;
}

bool AActor::isActorHasStaticMesh() const noexcept
{
    return false;
}

bool AActor::isActorHasCollision() const noexcept
{
    return false;
}

StaticMesh* AActor::GetStaticMesh() noexcept
{
    return nullptr;
}

Collision* AActor::GetCollision() noexcept
{
    return nullptr;
}

bool AActor::isActorHasLight() const noexcept
{
    return false;
}

Light* AActor::GetLight() noexcept
{
    return nullptr;
}

bool AActor::isActorHasCubemap() const noexcept
{
    return false;
}
void AActor::AttachCubemap()
{ }

void AActor::AddFuncToTrigger(AActor* trigger, int typeOfTrigger, AdditionalTriggerInfo* addTriggerInfo)
{ }