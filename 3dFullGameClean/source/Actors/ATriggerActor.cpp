
#include "ATriggerActor.h"
#include "ACameraActor.h"

extern std::vector<AActor*> actors;

ATriggerActor::ATriggerActor(StaticMesh* staticMesh, COORDS observableObjCentre, Range triggerRange)
    : observableObjCentre(observableObjCentre)
    , staticMesh(staticMesh)
    , triggerRange(triggerRange)
{ }

bool ATriggerActor::IsDotInRange(COORDS dot, Range range) const
{
    return (dot.x >= range.xStart && dot.x <= range.xEnd &&
        dot.y >= range.yStart && dot.y <= range.yEnd &&
        dot.z >= range.zStart && dot.z <= range.zEnd);
}

void ATriggerActor::BeginPlay()
{
    if (observableObjCentre.x == 0 && observableObjCentre.y == 0 && observableObjCentre.z == 0)
        observable = actors[0];
    else
    {
        float minDist = 10000;
        int index = 0;
        for (size_t i = 1; i < actors.size(); ++i)
        {
            if (actors[i]->isActorHasStaticMesh())
            {
                if (dotDistance(observableObjCentre, actors[i]->GetStaticMesh()->GetCentreCoord()) < minDist)
                {
                    minDist = dotDistance(observableObjCentre, actors[i]->GetStaticMesh()->GetCentreCoord());
                    index = i;
                }
            }
        }

        observable = actors[index];
    }
}

void ATriggerActor::Tick()
{
    COORDS currentObservableObjCoords;
    if (observable == actors[0])
        currentObservableObjCoords = ((ACamerasActor*)actors[0])->GetCurrentCamera()->GetCameraCoord();
    else
        currentObservableObjCoords = observable->GetStaticMesh()->GetCentreCoord();

    if (IsDotInRange(currentObservableObjCoords, triggerRange))
    {
        isOnMoveToDotTriggerActive = true;
    }
    if (isOnMoveToDotTriggerActive)
    {
        onMoveToDot.Broadcast();
    }

    if (IsDotInRange(currentObservableObjCoords, triggerRange))
    {
        isOnTeleportToDotTriggerActive = true;
    }
    if (isOnTeleportToDotTriggerActive)
    {
        onTeleportToDot.Broadcast();
    }

    if (IsDotInRange(currentObservableObjCoords, triggerRange))
    {
        isOnObjectRenderOffTriggerActive = true;
    }
    if (isOnObjectRenderOffTriggerActive)
    {
        onObjectRenderOff.Broadcast();
    }

    if (IsDotInRange(currentObservableObjCoords, triggerRange))
    {
        isOnEnvShakeOnTriggerActive = true;
    }
    if (isOnEnvShakeOnTriggerActive)
    {
        onEnvShakeOn.Broadcast();
    }

    if (IsDotInRange(currentObservableObjCoords, triggerRange))
    {
        isOnEnvShakeOffTriggerActive = true;
    }
    if (isOnEnvShakeOffTriggerActive)
    {
        onEnvShakeOff.Broadcast();
    }

    if (IsDotInRange(currentObservableObjCoords, triggerRange))
    {
        isOnEnvFadeOnTriggerActive = true;
    }
    if (isOnEnvFadeOnTriggerActive)
    {
        onEnvFadeOn.Broadcast();
    }

    if (IsDotInRange(currentObservableObjCoords, triggerRange))
    {
        isOnEnvFadeOffTriggerActive = true;
    }
    if (isOnEnvFadeOffTriggerActive)
    {
        onEnvFadeOff.Broadcast();
    }
}