
#include "ACameraShaker.h"
#include "Model.h"
#include "Circle.h"
#include "ACameraActor.h"

extern ACamerasActor camActor;

ACameraShaker::ACameraShaker(float shakePower) : rad(shakePower), isShakerActive(false)
{ }

void ACameraShaker::BeginPlay()
{ }

void ACameraShaker::Tick()
{
    if (!isShakerActive) return;

    COORDS endDot;
    endDot.x = -rad + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (rad + rad)));
    endDot.y = -sqrt(pow(rad, 2) - pow(endDot.x, 2)) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * sqrt(pow(rad, 2) - pow(endDot.x, 2)))));
    endDot.z = 0;

    endDot = endDot + rad;

    COORDS verCamDir = camActor.GetCurrentCamera()->GetVerCamDir();
    COORDS up = GetVectorRotateZOnAngle(verCamDir, 90);
    COORDS down = up * (-1);
    COORDS left = { up.y * verCamDir.z - up.z * verCamDir.y, up.z * verCamDir.x - up.x * verCamDir.z, up.x * verCamDir.y - up.y * verCamDir.x };
    COORDS right = left * (-1);

    COORDS localCentre = (right * rad + camActor.GetCurrentCamera()->GetCameraCoord()) + up * rad;
    camActor.GetCurrentCamera()->SetCameraCoordForRender() = left * endDot.x + up * endDot.y + camActor.GetCurrentCamera()->GetCameraCoord();
}