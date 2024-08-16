#pragma once

#include "Light.h"
#include "StaticMesh.h"
#include "Collision.h"

struct AdditionalTriggerInfo;

class AActor
{
public:
    virtual bool isActorHasStaticMesh() const noexcept;

    virtual bool isActorHasCollision() const noexcept;

    virtual StaticMesh* GetStaticMesh() noexcept;

    virtual Collision* GetCollision() noexcept;

    virtual bool isActorHasLight() const noexcept;

    virtual Light* GetLight() noexcept;

    virtual bool isActorHasCubemap() const noexcept;

    virtual void AttachCubemap();

    virtual float& GetCurrentRotateAngle() noexcept;

    virtual void AddFuncToTrigger(AActor* trigger, int typeOfTrigger, AdditionalTriggerInfo* addTriggerInfo);

    virtual void ImageTick();

    virtual void BeginPlay() = 0;

    virtual void Tick() = 0;
};