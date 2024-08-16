
#include "ACamera.h"

ACamera::ACamera()
    : cameraCoord({ 0,0,0 })
    , cameraCoordForRender({ 0,0,0 })
    , horCameraDirection(StartHorCameraDirection)
    , verCameraDirection(StartVerCameraDirection)
    , lastMoveVector({ 0,0,0 })
{ }

ACamera::ACamera(COORDS cameraCoord)
    : cameraCoord(cameraCoord)
    , cameraCoordForRender(cameraCoord)
    , horCameraDirection(StartHorCameraDirection)
    , verCameraDirection(StartVerCameraDirection)
    , lastMoveVector({ 0,0,0 })
{ }

COORDS ACamera::GetVerCamDir() const noexcept
{
    return verCameraDirection;
}

COORDS& ACamera::SetVerCamDir() noexcept
{
    return verCameraDirection;
}

COORDS ACamera::GetHorCamDir() const noexcept
{
    return horCameraDirection;
}

COORDS& ACamera::SetHorCamDir() noexcept
{
    return horCameraDirection;
}

COORDS ACamera::GetCameraCoord() const noexcept
{
    return cameraCoord;
}

COORDS& ACamera::SetCameraCoord() noexcept
{
    return cameraCoord;
}

COORDS ACamera::GetCameraCoordForRender() const noexcept
{
    return cameraCoordForRender;
}

COORDS& ACamera::SetCameraCoordForRender() noexcept
{
    return cameraCoordForRender;
}

COORDS& ACamera::SetLastMoveVector() noexcept
{
    return lastMoveVector;
}

COORDS ACamera::GetLastMoveVector() const noexcept
{
    return lastMoveVector;
}

float ACamera::GetCameraHorAngleOnNewSurface() const noexcept
{
    return HorAngleOnNewSurface;
}

void ACamera::CalcCameraHorAngleOnNewSurface()
{
    static const COORDS plusXAxis = { 0.1F, 0, 0 };
    static const COORDS minusXAxis = { -0.1F, 0, 0 };
    static const COORDS plusYAxis = { 0, 0.1F, 0 };
    static const COORDS minusYAxis = { 0, -0.1F, 0 };

    HorAngleOnNewSurface = HorAngleOnNewSurfaceErrorValue;

    COORDS plusXNewSurfaceAxis = dotProjectionOnSurface(verCameraDirection, GetVectorRotateXYOnAngle(verCameraDirection, 10), plusXAxis);
    COORDS plusYNewSurfaceAxis = dotProjectionOnSurface(verCameraDirection, GetVectorRotateXYOnAngle(verCameraDirection, 10), plusYAxis);
    COORDS minusXNewSurfaceAxis = dotProjectionOnSurface(verCameraDirection, GetVectorRotateXYOnAngle(verCameraDirection, 10), minusXAxis);
    COORDS minusYNewSurfaceAxis = dotProjectionOnSurface(verCameraDirection, GetVectorRotateXYOnAngle(verCameraDirection, 10), minusYAxis);

    if ((GetAngleBetweenVectors({ 0,0,0 }, plusXNewSurfaceAxis, verCameraDirection)) <= 90 && (GetAngleBetweenVectors({ 0,0,0 }, plusYNewSurfaceAxis, verCameraDirection)) <= 90)
        HorAngleOnNewSurface = GetAngleBetweenVectors({ 0,0,0 }, plusXNewSurfaceAxis, verCameraDirection);
    else if ((GetAngleBetweenVectors({ 0,0,0 }, plusYNewSurfaceAxis, verCameraDirection)) <= 90 && (GetAngleBetweenVectors({ 0,0,0 }, minusXNewSurfaceAxis, verCameraDirection)) <= 90)
        HorAngleOnNewSurface = GetAngleBetweenVectors({ 0,0,0 }, plusYNewSurfaceAxis, verCameraDirection) + 90;
    else if ((GetAngleBetweenVectors({ 0,0,0 }, minusXNewSurfaceAxis, verCameraDirection)) <= 90 && (GetAngleBetweenVectors({ 0,0,0 }, minusYNewSurfaceAxis, verCameraDirection)) <= 90)
        HorAngleOnNewSurface = GetAngleBetweenVectors({ 0,0,0 }, minusXNewSurfaceAxis, verCameraDirection) + 180;
    else if ((GetAngleBetweenVectors({ 0,0,0 }, plusXNewSurfaceAxis, verCameraDirection)) <= 90 && (GetAngleBetweenVectors({ 0,0,0 }, minusYNewSurfaceAxis, verCameraDirection)) <= 90)
        HorAngleOnNewSurface = GetAngleBetweenVectors({ 0,0,0 }, minusYNewSurfaceAxis, verCameraDirection) + 270;
}