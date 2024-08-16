
#include "ABaseActor.h"
#include "Model.h"
#include "Circle.h"
#include "ACameraActor.h"

extern ACamerasActor camActor;

ABaseActor::ABaseActor(StaticMesh* staticMesh) : staticMesh(staticMesh)
{ }

float& ABaseActor::GetCurrentRotateAngle() noexcept
{
    return currentRotateAngle;
}

void ABaseActor::BeginPlay()
{
    if (staticMesh->GetObjType() == objectType::MODEL && (((Model*)staticMesh))->GetAllAnimModels().size() > 1)
        reinterpret_cast<Model*>(staticMesh)->RotationAllAnimations(90, Z_AXIS_ROTATION_KEY);
}

void ABaseActor::Tick()
{
    if (staticMesh->GetObjType() == objectType::MODEL)
        reinterpret_cast<Model*>(staticMesh)->AnimationHandle();
    else if (staticMesh->GetObjType() == objectType::CIRCLE)
    {
        reinterpret_cast<Circle*>(staticMesh)->SetNormVec({ camActor.GetCurrentCamera()->GetCameraCoord().x, camActor.GetCurrentCamera()->GetCameraCoord().y, staticMesh->GetCentreCoord().z});
        reinterpret_cast<Circle*>(staticMesh)->AnimationHandle();
    }
}