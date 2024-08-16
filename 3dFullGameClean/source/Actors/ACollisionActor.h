#pragma once

#include "Preprocessor.h"
#include "AActor.h"
#include "Collision.h"
#include "ATriggerActor.h"

extern double tick;

class ACollisionActor : public AActor
{
    // Trigger Part Functions
private:
    //Move to dot vars
    COORDS pos = {0,0,0}, rate = { 0,0,0 };

    void MoveToDot(COORDS endDot, float speed, bool& isOnMoveToDotTriggerActive)
    {
        if (pos == COORDS{ 0,0,0 }) pos = staticMesh->GetCentreCoord();

        if (pos.x >= endDot.x - 0.1 && pos.x <= endDot.x + 0.1)
        {
            pos.x = staticMesh->GetCentreCoord().x;
            rate.x = 0;
        }
        else
            MotionCubic(endDot.x, tick * speed, &pos.x, &rate.x);

        if (pos.y >= endDot.y - 0.1 && pos.y <= endDot.y + 0.1)
        {
            pos.y = staticMesh->GetCentreCoord().y;
            rate.y = 0;
        }
        else
            MotionCubic(endDot.y, tick * speed, &pos.y, &rate.y);

        if (pos.z >= endDot.z - 0.1 && pos.z <= endDot.z + 0.1)
        {
            pos.z = staticMesh->GetCentreCoord().z;
            rate.z = 0;
        }
        else
            MotionCubic(endDot.z, tick * speed, &pos.z, &rate.z);

        if (pos == staticMesh->GetCentreCoord())
        {
            isOnMoveToDotTriggerActive = false;
            return;
        }

        staticMesh->SetCentreCoord() = pos;
        staticMesh->UpdateRange();
        if (staticMesh->GetObjType() == objectType::PARALLELEPIPED && staticMesh->IsRotate())
            staticMesh->Rotation(0.0001f);
    }

    void TeleportToDot(COORDS endDot, bool& isOnTeleportToDotTriggerActive)
    {
        staticMesh->SetCentreCoord() = endDot;

        staticMesh->UpdateRange();
        if (staticMesh->GetObjType() == objectType::PARALLELEPIPED && staticMesh->IsRotate())
            staticMesh->Rotation(0.0001f);

        isOnTeleportToDotTriggerActive = false;
    }

    void ObjectRenderOff(bool& isOnObjectRenderOffTriggerActive)
    {
        staticMesh->SetRenderMode(false);
        isOnObjectRenderOffTriggerActive = false;
    }

    void AddFuncToTrigger(AActor* trigger, int typeOfTrigger, AdditionalTriggerInfo* addTriggerInfo) override
    {
        ATriggerActor* triggerActor = (ATriggerActor*)trigger;
        if (typeOfTrigger == static_cast<int>(ATriggerActor::TypesOfTrigger::MOVE_TO_DOT))
            triggerActor->onMoveToDot.AddUObject(this, &ACollisionActor::MoveToDot, ((ATriggerActor::MoveToDotInfo*)addTriggerInfo)->endDot, ((ATriggerActor::MoveToDotInfo*)addTriggerInfo)->speed, triggerActor->isOnMoveToDotTriggerActive);
        else if (typeOfTrigger == static_cast<int>(ATriggerActor::TypesOfTrigger::TELEPORT_TO_DOT))
            triggerActor->onTeleportToDot.AddUObject(this, &ACollisionActor::TeleportToDot, ((ATriggerActor::TeleportToDotInfo*)addTriggerInfo)->endDot, triggerActor->isOnTeleportToDotTriggerActive);
        else if (typeOfTrigger == static_cast<int>(ATriggerActor::TypesOfTrigger::OBJECT_RENDER_OFF))
            triggerActor->onObjectRenderOff.AddUObject(this, &ACollisionActor::ObjectRenderOff, triggerActor->isOnObjectRenderOffTriggerActive);
    }

private:
    UPROPERTY(StaticMesh, staticMesh)
    StaticMesh* staticMesh;

    UPROPERTY(Collision, collision)
    Collision* collision;
    
    DEFINE_CUBEMAP()

    float currentRotateAngle = 0;

public:
    ACollisionActor(StaticMesh* staticMesh, Collision* collision);

    float& GetCurrentRotateAngle() noexcept override;

    void BeginPlay() override;

    void Tick() override;
};