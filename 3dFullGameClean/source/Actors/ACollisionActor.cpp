
#include "ACollisionActor.h"

ACollisionActor::ACollisionActor(StaticMesh* staticMesh, Collision* collision) : staticMesh(staticMesh), collision(collision)
{ }

float& ACollisionActor::GetCurrentRotateAngle() noexcept
{
    return currentRotateAngle;
}

void ACollisionActor::BeginPlay()
{ }

void ACollisionActor::Tick()
{ }