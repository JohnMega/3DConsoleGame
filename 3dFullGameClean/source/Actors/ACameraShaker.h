#pragma once

#include "Preprocessor.h"
#include "AActor.h"
#include "Model.h"
#include "ATriggerActor.h"

class ACameraShaker : public AActor
{
private:
    void EnvShakeOn(bool& isOnEnvShakeOnTriggerActive)
    {
        isShakerActive = true;
        isOnEnvShakeOnTriggerActive = false;
    }

    void EnvShakeOff(bool& isOnEnvShakeOffTriggerActive)
    {
        isShakerActive = false;
        isOnEnvShakeOffTriggerActive = false;
    }

    void AddFuncToTrigger(AActor* trigger, int typeOfTrigger, AdditionalTriggerInfo* addTriggerInfo) override
    {
        ATriggerActor* triggerActor = (ATriggerActor*)trigger;
        if (typeOfTrigger == static_cast<int>(ATriggerActor::TypesOfTrigger::ENV_SHAKE_ON))
            triggerActor->onEnvShakeOn.AddUObject(this, &ACameraShaker::EnvShakeOn, triggerActor->isOnEnvShakeOnTriggerActive);
        else if (typeOfTrigger == static_cast<int>(ATriggerActor::TypesOfTrigger::ENV_SHAKE_OFF))
            triggerActor->onEnvShakeOff.AddUObject(this, &ACameraShaker::EnvShakeOff, triggerActor->isOnEnvShakeOffTriggerActive);
    }
    
private:
    float rad = 0.4f;
    bool isShakerActive;

public:
    ACameraShaker(float shakePower);

    void BeginPlay() override;

    void Tick() override;
};