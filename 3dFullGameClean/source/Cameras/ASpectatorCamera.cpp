
#include "ASpectatorCamera.h"
#include <cmath>
#include "Maths.h"

void ASpectatorCamera::Move() noexcept
{
    if (NewMouseCoords.x < OldMouseCoords.x) CamRotateOnXYAxis(360 - abs(NewMouseCoords.x - OldMouseCoords.x) / (float)3);
    if (NewMouseCoords.x > OldMouseCoords.x) CamRotateOnXYAxis(abs(NewMouseCoords.x - OldMouseCoords.x) / (float)3);
    if (NewMouseCoords.y < OldMouseCoords.y) CamRotateOnZAxis(abs(NewMouseCoords.y - OldMouseCoords.y) / (float)3);
    if (NewMouseCoords.y > OldMouseCoords.y) CamRotateOnZAxis(360 - abs(NewMouseCoords.y - OldMouseCoords.y) / (float)3);
    //OldMouseCoords = NewMouseCoords;

    if (key == FORWARD)
    {
        cameraCoord += {verCameraDirection.x, verCameraDirection.y, verCameraDirection.z};
        lastMoveVector = verCameraDirection;
    }

    if (key == BACK)
    {
        cameraCoord -= {verCameraDirection.x, verCameraDirection.y, verCameraDirection.z};
        lastMoveVector = verCameraDirection * (-1);
    }

    if (key == RIGHT)
    {
        rightCameraDirection = GetVectorRotateXYOnAngle(verCameraDirection, 90);
        cameraCoord += {rightCameraDirection.x, rightCameraDirection.y, rightCameraDirection.z};
        lastMoveVector = rightCameraDirection;
    }

    if (key == LEFT)
    {
        rightCameraDirection = GetVectorRotateXYOnAngle(verCameraDirection, 90);
        cameraCoord -= {rightCameraDirection.x, rightCameraDirection.y, rightCameraDirection.z};
        lastMoveVector = rightCameraDirection * (-1);
    }

    key = NULL;

#ifdef DEBUG
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { SCREEN_WIDTH + 2, 3 });
    std::cout << "                          ";
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { SCREEN_WIDTH + 2, 3 });
    std::cout << HorAngleOnNewSurface;
#endif
}

void ASpectatorCamera::CamRotateOnZAxis(float angle)
{
    static const float VerCamDirZAxisMaxAllowValue = 0.95;
    static const float VerCamDirZAxisMinAllowValue = -0.95;
    static const float VerCamDirZAxisNotAllowIntervalStart = -0.05;
    static const float VerCamDirZAxisNotAllowIntervalEnd = 0.05;

    if ((NewMouseCoords.y < OldMouseCoords.y) ? verCameraDirection.z < VerCamDirZAxisMaxAllowValue : verCameraDirection.z > VerCamDirZAxisMinAllowValue)
    {
        verCameraDirection = GetVectorRotateZOnAngle(verCameraDirection, angle);

        CalcCameraHorAngleOnNewSurface();

        //while (HorAngleOnNewSurface == HorAngleOnNewSurfaceErrorValue || (verCameraDirection.z >= VerCamDirZAxisNotAllowIntervalStart && verCameraDirection.z <= VerCamDirZAxisNotAllowIntervalEnd))
        while (HorAngleOnNewSurface == HorAngleOnNewSurfaceErrorValue)
        {
            verCameraDirection = GetVectorRotateZOnAngle(verCameraDirection, angle);
            CalcCameraHorAngleOnNewSurface();
        }
    }
}

void ASpectatorCamera::CamRotateOnXYAxis(float angle)
{
    horCameraDirection = GetVectorRotateXYOnAngle(horCameraDirection, angle);
    verCameraDirection = { GetVectorRotateXYOnAngle(verCameraDirection, angle).x, GetVectorRotateXYOnAngle(verCameraDirection, angle).y, GetVectorRotateXYOnAngle(verCameraDirection, angle).z / cos(fromDegreesToRad(angle)) };

    CalcCameraHorAngleOnNewSurface();

    bool isHorAngleOnNewSurfaceHaveBadV = (HorAngleOnNewSurface >= 359 || HorAngleOnNewSurface <= 1) || (HorAngleOnNewSurface >= 89 && HorAngleOnNewSurface <= 91) || (HorAngleOnNewSurface >= 179 && HorAngleOnNewSurface <= 181) || (HorAngleOnNewSurface >= 269 && HorAngleOnNewSurface <= 271);
    while (HorAngleOnNewSurface == HorAngleOnNewSurfaceErrorValue || isHorAngleOnNewSurfaceHaveBadV)
    {
        horCameraDirection = GetVectorRotateXYOnAngle(horCameraDirection, angle);
        verCameraDirection = { GetVectorRotateXYOnAngle(verCameraDirection, angle).x, GetVectorRotateXYOnAngle(verCameraDirection, angle).y, GetVectorRotateXYOnAngle(verCameraDirection, angle).z / cos(fromDegreesToRad(angle)) };
        CalcCameraHorAngleOnNewSurface();

        isHorAngleOnNewSurfaceHaveBadV = (HorAngleOnNewSurface >= 359 || HorAngleOnNewSurface <= 1) || (HorAngleOnNewSurface >= 89 && HorAngleOnNewSurface <= 91) || (HorAngleOnNewSurface >= 179 && HorAngleOnNewSurface <= 181) || (HorAngleOnNewSurface >= 269 && HorAngleOnNewSurface <= 271);
    }
}