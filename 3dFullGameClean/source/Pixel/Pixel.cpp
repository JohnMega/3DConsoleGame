
#include <cmath>
#include "Pixel.h"
#include "ACamera.h"
#include "GlobalVars.h"

Pixel::Pixel() noexcept
    : ScreenPosX(NULL)
    , ScreenPosY(NULL)
    , dotCoord{ NULL }
{ }

Pixel::Pixel(COORDS dotCoord) noexcept
    : ScreenPosX(NULL)
    , ScreenPosY(NULL)
    , dotCoord(dotCoord)
{ }

float Pixel::GetScreenPosX() const noexcept
{
    return ScreenPosX;
}

float Pixel::GetScreenPosY() const noexcept
{
    return ScreenPosY;
}

COORDS& Pixel::SetDotCoord() noexcept
{
    return dotCoord;
}

void Pixel::ScreenPosXCalc(const ACamera& playerCamera, COORDS firstQuaterAxis, COORDS secondQuaterAxis)
{
    COORDS cameraCoord = playerCamera.GetCameraCoordForRender();
    COORDS verCamDir = playerCamera.GetVerCamDir();

    COORDS projectedDotOnCamHorDirSurface = cameraCoord + dotProjectionOnSurface(verCamDir, GetVectorRotateXYOnAngle(verCamDir, 10), dotCoord - cameraCoord);

    float angleBetweenVerCamDirAndSecondQuaterAxis = GetAngleBetweenVectors(cameraCoord, cameraCoord + verCamDir, secondQuaterAxis);

    if ((GetAngleBetweenVectors(cameraCoord, firstQuaterAxis, projectedDotOnCamHorDirSurface)) >= 90)
        ScreenPosX = ((SCREEN_WIDTH - 2) / 2) - floor(GetAngleBetweenVectors(cameraCoord, cameraCoord + verCamDir, projectedDotOnCamHorDirSurface));
    else if ((GetAngleBetweenVectors(cameraCoord, secondQuaterAxis, projectedDotOnCamHorDirSurface)) >= 90)
        ScreenPosX = ((SCREEN_WIDTH - 2) / 2) + floor(GetAngleBetweenVectors(cameraCoord, cameraCoord + verCamDir, projectedDotOnCamHorDirSurface));
    else
        ScreenPosX = ((SCREEN_WIDTH - 2) / 2) + (((GetAngleBetweenVectors(cameraCoord, secondQuaterAxis, projectedDotOnCamHorDirSurface)) >= angleBetweenVerCamDirAndSecondQuaterAxis) ?
            floor(GetAngleBetweenVectors(cameraCoord, cameraCoord + verCamDir, projectedDotOnCamHorDirSurface)) : -floor(GetAngleBetweenVectors(cameraCoord, cameraCoord + verCamDir, projectedDotOnCamHorDirSurface)));
}

void Pixel::ScreenPosYCalc(const ACamera& playerCamera)
{
    COORDS cameraCoord = playerCamera.GetCameraCoordForRender();
    COORDS verCamDir = playerCamera.GetVerCamDir();

    COORDS projectedDotOnVerCamDirSurface = cameraCoord + dotProjectionOnSurface(verCamDir, GetVectorRotateZOnAngle(verCamDir, 10), dotCoord - cameraCoord);
    COORDS projectedVerCamDirOnXYSurface = cameraCoord + dotProjectionOnSurface(playerCamera.GetHorCamDir(), GetVectorRotateXYOnAngle(playerCamera.GetHorCamDir(), 10), verCamDir);

    float angleBetweenVerCamDirAndXYVerCamDir = GetAngleBetweenVectors(cameraCoord, cameraCoord + verCamDir, projectedVerCamDirOnXYSurface);
    int partOfTheScreenChoice = ((verCamDir.z <= 0) ? 1 : -1);

    if (GetAngleBetweenVectors(cameraCoord, { cameraCoord.x, cameraCoord.y, (verCamDir.z <= 0) ? cameraCoord.z - 0.1F : cameraCoord.z + 0.1F }, projectedDotOnVerCamDirSurface) > 90)
        ScreenPosY = (SCREEN_HEIGHT / 2) - partOfTheScreenChoice * floor(GetAngleBetweenVectors(cameraCoord, cameraCoord + verCamDir, projectedDotOnVerCamDirSurface) / 2);
    else if ((GetAngleBetweenVectors(cameraCoord, projectedDotOnVerCamDirSurface, projectedVerCamDirOnXYSurface)) >= 90)
        ScreenPosY = (SCREEN_HEIGHT / 2) + partOfTheScreenChoice * floor(GetAngleBetweenVectors(cameraCoord, cameraCoord + verCamDir, projectedDotOnVerCamDirSurface) / 2);
    else
        ScreenPosY = (SCREEN_HEIGHT / 2) + partOfTheScreenChoice * (((GetAngleBetweenVectors(cameraCoord, projectedVerCamDirOnXYSurface, projectedDotOnVerCamDirSurface)) > angleBetweenVerCamDirAndXYVerCamDir) ?
            floor(GetAngleBetweenVectors(cameraCoord, cameraCoord + verCamDir, projectedDotOnVerCamDirSurface) / 2) : -floor(GetAngleBetweenVectors(cameraCoord, cameraCoord + verCamDir, projectedDotOnVerCamDirSurface) / 2));
}

void Pixel::CalcDotCoordsOnScreen(const ACamera& playerCamera)
{
    ScreenPosXCalcPart(playerCamera);
    ScreenPosYCalcPart(playerCamera);
}

void Pixel::ScreenPosXCalcPart(const ACamera& playerCamera)
{
    static const COORDS plusXAxis = { 0.1F, 0, 0 };
    static const COORDS minusXAxis = { -0.1F, 0, 0 };
    static const COORDS plusYAxis = { 0, 0.1F, 0 };
    static const COORDS minusYAxis = { 0, -0.1F, 0 };

    COORDS cameraCoord = playerCamera.GetCameraCoordForRender();
    COORDS verCamDir = playerCamera.GetVerCamDir();

    COORDS plusXNewSurfaceAxis = cameraCoord + dotProjectionOnSurface(verCamDir, GetVectorRotateXYOnAngle(verCamDir, 10), plusXAxis);
    COORDS plusYNewSurfaceAxis = cameraCoord + dotProjectionOnSurface(verCamDir, GetVectorRotateXYOnAngle(verCamDir, 10), plusYAxis);
    COORDS minusXNewSurfaceAxis = cameraCoord + dotProjectionOnSurface(verCamDir, GetVectorRotateXYOnAngle(verCamDir, 10), minusXAxis);
    COORDS minusYNewSurfaceAxis = cameraCoord + dotProjectionOnSurface(verCamDir, GetVectorRotateXYOnAngle(verCamDir, 10), minusYAxis);

    if (playerCamera.GetCameraHorAngleOnNewSurface() >= 0 && playerCamera.GetCameraHorAngleOnNewSurface() <= 90)
    {
        ScreenPosXCalc(playerCamera, plusYNewSurfaceAxis, plusXNewSurfaceAxis);
    }
    else if (playerCamera.GetCameraHorAngleOnNewSurface() > 90 && playerCamera.GetCameraHorAngleOnNewSurface() <= 180)
    {
        ScreenPosXCalc(playerCamera, minusXNewSurfaceAxis, plusYNewSurfaceAxis);
    }
    else if (playerCamera.GetCameraHorAngleOnNewSurface() > 180 && playerCamera.GetCameraHorAngleOnNewSurface() <= 270)
    {
        ScreenPosXCalc(playerCamera, minusYNewSurfaceAxis, minusXNewSurfaceAxis);
    }
    else if (playerCamera.GetCameraHorAngleOnNewSurface() > 270 && playerCamera.GetCameraHorAngleOnNewSurface() <= 360)
    {
        ScreenPosXCalc(playerCamera, plusXNewSurfaceAxis, minusYNewSurfaceAxis);
    }
}

void Pixel::ScreenPosYCalcPart(const ACamera& playerCamera)
{
    ScreenPosYCalc(playerCamera);
}