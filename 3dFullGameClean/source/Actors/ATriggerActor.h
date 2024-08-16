#pragma once
#include "Maths.h"
#include "Preprocessor.h"
#include "AActor.h"
#include "Delegate.h"

DECLARE_DELEGATE(FOnMoveToDot, COORDS, float, bool&);
DECLARE_DELEGATE(FOnTeleportToDot, COORDS, bool&);
DECLARE_DELEGATE(FOnObjectRenderOff, bool&);
DECLARE_DELEGATE(FOnEnvShakeOn, bool&);
DECLARE_DELEGATE(FOnEnvShakeOff, bool&);
DECLARE_DELEGATE(FOnEnvFadeOn, bool&);
DECLARE_DELEGATE(FOnEnvFadeOff, bool&);

struct AdditionalTriggerInfo
{
    static AdditionalTriggerInfo* ReadInfo(char*& ptr);
};

class ATriggerActor : public AActor
{
public:
    enum class TypesOfTrigger : char { MOVE_TO_DOT, TELEPORT_TO_DOT, OBJECT_RENDER_OFF, ENV_SHAKE_ON, ENV_SHAKE_OFF, ENV_FADE_ON, ENV_FADE_OFF };

    struct MoveToDotInfo : AdditionalTriggerInfo
    {
    public:
        COORDS endDot;
        float speed;

    public:
        MoveToDotInfo() : endDot({ 0,0,0 }), speed(1)
        { }

        MoveToDotInfo(COORDS endDot, float speed) : endDot(endDot), speed(speed)
        { }

        static AdditionalTriggerInfo* ReadInfo(char*& ptr)
        {
            COORDS endDotOutput;
            endDotOutput.x = atof(ptr); while (*ptr++ != ';') {}
            endDotOutput.y = atof(ptr); while (*ptr++ != ';') {}
            endDotOutput.z = atof(ptr); while (*ptr++ != ';') {}

            float speed;
            speed = atof(ptr); while (*ptr++ != '}') {}

            return (new MoveToDotInfo(endDotOutput, speed));
        }
    };

    struct TeleportToDotInfo : AdditionalTriggerInfo
    {
    public:
        COORDS endDot;

    public:
        TeleportToDotInfo() : endDot({ 0,0,0 })
        { }

        TeleportToDotInfo(COORDS endDot) : endDot(endDot)
        { }

        static AdditionalTriggerInfo* ReadInfo(char*& ptr)
        {
            COORDS endDotOutput;
            endDotOutput.x = atof(ptr); while (*ptr++ != ';') {}
            endDotOutput.y = atof(ptr); while (*ptr++ != ';') {}
            endDotOutput.z = atof(ptr); while (*ptr++ != '}') {}

            return (new TeleportToDotInfo(endDotOutput));
        }
    };

    struct ObjectRenderOffInfo : AdditionalTriggerInfo
    {
        ObjectRenderOffInfo()
        { }

        ObjectRenderOffInfo(COORDS endDot)
        { }

        static AdditionalTriggerInfo* ReadInfo(char*& ptr)
        {
            ptr += 2;

            return (new ObjectRenderOffInfo);
        }
    };

    struct EnvShakeOnInfo : AdditionalTriggerInfo
    {
        EnvShakeOnInfo()
        { }

        EnvShakeOnInfo(COORDS endDot)
        { }

        static AdditionalTriggerInfo* ReadInfo(char*& ptr)
        {
            ptr += 2;

            return (new EnvShakeOnInfo);
        }
    };

    struct EnvShakeOffInfo : AdditionalTriggerInfo
    {
        EnvShakeOffInfo()
        { }

        EnvShakeOffInfo(COORDS endDot)
        { }

        static AdditionalTriggerInfo* ReadInfo(char*& ptr)
        {
            ptr += 2;

            return (new EnvShakeOffInfo);
        }
    };

    struct EnvFadeOnInfo : AdditionalTriggerInfo
    {
        EnvFadeOnInfo()
        { }

        EnvFadeOnInfo(COORDS endDot)
        { }

        static AdditionalTriggerInfo* ReadInfo(char*& ptr)
        {
            ptr += 2;

            return (new EnvFadeOnInfo);
        }
    };

    struct EnvFadeOffInfo : AdditionalTriggerInfo
    {
        EnvFadeOffInfo()
        { }

        EnvFadeOffInfo(COORDS endDot)
        { }

        static AdditionalTriggerInfo* ReadInfo(char*& ptr)
        {
            ptr += 2;

            return (new EnvFadeOffInfo);
        }
    };

    using funcPtrType = AdditionalTriggerInfo * (*)(char*&);
    static inline const std::vector<funcPtrType> ReadInfoFuncTable = { (&MoveToDotInfo::ReadInfo), (&TeleportToDotInfo::ReadInfo)
        , (&ObjectRenderOffInfo::ReadInfo), (&EnvShakeOnInfo::ReadInfo), (&EnvShakeOffInfo::ReadInfo), (&EnvFadeOnInfo::ReadInfo)
        , (&EnvFadeOffInfo::ReadInfo) };

private:
    UPROPERTY(StaticMesh, staticMesh)
    StaticMesh* staticMesh;

    Range triggerRange;
    AActor* observable;
    COORDS observableObjCentre;

private:
    bool IsDotInRange(COORDS dot, Range range) const;

public:
    FOnMoveToDot onMoveToDot;
    bool isOnMoveToDotTriggerActive = false;

    FOnTeleportToDot onTeleportToDot;
    bool isOnTeleportToDotTriggerActive = false;

    FOnObjectRenderOff onObjectRenderOff;
    bool isOnObjectRenderOffTriggerActive = false;

    FOnEnvShakeOn onEnvShakeOn;
    bool isOnEnvShakeOnTriggerActive = false;

    FOnEnvShakeOff onEnvShakeOff;
    bool isOnEnvShakeOffTriggerActive = false;

    FOnEnvFadeOn onEnvFadeOn;
    bool isOnEnvFadeOnTriggerActive = false;

    FOnEnvFadeOff onEnvFadeOff;
    bool isOnEnvFadeOffTriggerActive = false;

public:
    ATriggerActor(StaticMesh* staticMesh, COORDS observableObjCentre, Range triggerRange);

    void BeginPlay() override;

    void Tick() override;
};