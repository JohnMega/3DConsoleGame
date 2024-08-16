#pragma once

#include "Preprocessor.h"
#include "AActor.h"
#include "Model.h"
#include "ATriggerActor.h"

class AEnvFade : public AActor
{
private:
    void EnvFadeOn(bool& isOnEnvFadeOnTriggerActive)
    {
        if(!isOnEnvFadeOn)
        for (size_t i = 0; i < SCREEN_HEIGHT - 2; ++i)
            for (size_t j = 0; j < SCREEN_WIDTH - 2; ++j)
                rgbVecLenRatio[i][j] = { 0,0,0 };

        isOnEnvFadeOn = true;
        isOnEnvFadeOnTriggerActive = false;
    }

    void EnvFadeOff(bool& isOnEnvFadeOffTriggerActive)
    {
        if(!isOnEnvFadeOff)
        for (size_t i = 0; i < SCREEN_HEIGHT - 2; ++i)
            for (size_t j = 0; j < SCREEN_WIDTH - 2; ++j)
                rgbVecLenRatio[i][j] = { 1,1,1 };

        isOnEnvFadeOff = true;
        isOnEnvFadeOffTriggerActive = false;
    }

    void AddFuncToTrigger(AActor* trigger, int typeOfTrigger, AdditionalTriggerInfo* addTriggerInfo) override
    {
        ATriggerActor* triggerActor = (ATriggerActor*)trigger;
        if (typeOfTrigger == static_cast<int>(ATriggerActor::TypesOfTrigger::ENV_FADE_ON))
            triggerActor->onEnvFadeOn.AddUObject(this, &AEnvFade::EnvFadeOn, triggerActor->isOnEnvFadeOnTriggerActive);
        else if (typeOfTrigger == static_cast<int>(ATriggerActor::TypesOfTrigger::ENV_FADE_OFF))
            triggerActor->onEnvFadeOff.AddUObject(this, &AEnvFade::EnvFadeOff, triggerActor->isOnEnvFadeOffTriggerActive);
    }

private:
    static inline const float fadeOnDelay = 1;

    std::vector<std::vector<COORDS>> rgbVecLenRatio;
    float startRgbVecLen;
    bool isOnEnvFadeOff;

    float fadeOnDelaySum;
    bool isOnEnvFadeOn;

public:
    AEnvFade();

    void BeginPlay() override;

    void Tick() override;

    void ImageTick() override;
};