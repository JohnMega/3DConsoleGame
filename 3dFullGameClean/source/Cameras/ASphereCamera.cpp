
#include <iostream>
#include "ASphereCamera.h"
#include "Model.h"

extern double tick;

ASphereCamera::ASphereCamera(COORDS cameraCoord, StaticMesh* observedObject) : ACamera(cameraCoord)
{
    Position = cameraCoord.x;
    Rate = 0;

    SetObservedObject(observedObject);
}

void ASphereCamera::SetObservedObject(StaticMesh* observedObject) noexcept
{
    this->observedObject = observedObject;
}

void ASphereCamera::Move() noexcept
{
    COORDS sphereCamCoord = ToSphereFromCartesianCoords(cameraCoord);

    COORDS sphereObjCoord = ToSphereFromCartesianCoords({ observedObject->GetCentreCoord().x
        , observedObject->GetCentreCoord().y
        , observedObject->GetCentreCoord().z + (((Model*)observedObject)->GetModelRange().zEnd - ((Model*)observedObject)->GetModelRange().zStart) / 1.5f });

    if (observedObject->GetObjType() == objectType::MODEL)
    {
        if (!(Position >= sphereObjCoord.x - 0.01 && Position <= sphereObjCoord.x + 0.01))
        {
            MotionCubic(sphereObjCoord.x, tick, &Position, &Rate);

            cameraCoord = ToCartesianFromSphereCoords({ Position, 0.2f, sphereCamCoord.z });
        }
        else
            Rate = 0;
    }

    if (NewMouseCoords.x < OldMouseCoords.x && sphereCamCoord.y > -1.2) sphereCamCoord = { sphereCamCoord.x, sphereCamCoord.y - abs(NewMouseCoords.x - OldMouseCoords.x) / (float)50, sphereCamCoord.z };
    if (NewMouseCoords.x > OldMouseCoords.x && sphereCamCoord.y < 1.2) sphereCamCoord = { sphereCamCoord.x, sphereCamCoord.y + abs(NewMouseCoords.x - OldMouseCoords.x) / (float)50, sphereCamCoord.z };
    if (NewMouseCoords.y < OldMouseCoords.y && sphereCamCoord.z < 1.4) sphereCamCoord = { sphereCamCoord.x, sphereCamCoord.y, sphereCamCoord.z + abs(NewMouseCoords.y - OldMouseCoords.y) / (float)50 };
    if (NewMouseCoords.y > OldMouseCoords.y && sphereCamCoord.z > 0.1) sphereCamCoord = { sphereCamCoord.x, sphereCamCoord.y, sphereCamCoord.z - abs(NewMouseCoords.y - OldMouseCoords.y) / (float)50 };
    OldMouseCoords = NewMouseCoords;

    if ((Position >= sphereObjCoord.x - 0.01 && Position <= sphereObjCoord.x + 0.01))
        cameraCoord = ToCartesianFromSphereCoords(sphereCamCoord);
    verCameraDirection = vectorNormalize(observedObject->GetCentreCoord(), cameraCoord);

    CalcCameraHorAngleOnNewSurface();

    cameraCoordForRender = cameraCoord;
}