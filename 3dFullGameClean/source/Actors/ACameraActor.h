
#pragma once
#include "AActor.h"
#include "ACamera.h"
#include "ATriggerActor.h"

class ACamerasActor : public AActor
{
private:
    std::vector<ACamera*> cameras;
    std::vector<ATriggerActor*> triggers;
    size_t ptr;

public:
    ACamerasActor();

    void AddTrigger(ATriggerActor* trigger);

    void AddCamera(ACamera* camera) noexcept;

    ACamera* GetCurrentCamera() noexcept;

    void PopCamera() noexcept;

    void ChangeCamera() noexcept;

    void BeginPlay() override;

    void Tick() override;
};