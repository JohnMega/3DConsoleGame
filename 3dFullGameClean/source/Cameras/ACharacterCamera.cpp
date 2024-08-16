
#include "ACharacterCamera.h"
#include <cmath>
#include <iostream>
#include "Maths.h"

extern double tick;

void ACharacterCamera::Move() noexcept
{
    cameraCoordForRender = cameraCoord;

    if (NewMouseCoords.x < OldMouseCoords.x) CamRotateOnXYAxis(360 - abs(NewMouseCoords.x - OldMouseCoords.x) / (float)3);
    if (NewMouseCoords.x > OldMouseCoords.x) CamRotateOnXYAxis(abs(NewMouseCoords.x - OldMouseCoords.x) / (float)3);
    if (NewMouseCoords.y < OldMouseCoords.y) CamRotateOnZAxis(abs(NewMouseCoords.y - OldMouseCoords.y) / (float)3);
    if (NewMouseCoords.y > OldMouseCoords.y) CamRotateOnZAxis(360 - abs(NewMouseCoords.y - OldMouseCoords.y) / (float)3);
    //OldMouseCoords = NewMouseCoords;

    if (GetAsyncKeyState(0x57) & 0x8000)
    {
        cameraCoord += (horCameraDirection * 4) * tick;
        lastMoveVector = (horCameraDirection * 4) * tick;
    }
    else if (GetAsyncKeyState(0x53) & 0x8000)
    {
        cameraCoord -= (horCameraDirection * 4) * tick;
        lastMoveVector = ((horCameraDirection * 4) * tick) * (-1);
    }
    else if (GetAsyncKeyState(0x44) & 0x8000)
    {
        rightCameraDirection = GetVectorRotateXYOnAngle((horCameraDirection * 4) * tick, 90);
        cameraCoord += rightCameraDirection;
        lastMoveVector = rightCameraDirection;
    }
    else if (GetAsyncKeyState(0x41) & 0x8000)
    {
        rightCameraDirection = GetVectorRotateXYOnAngle((horCameraDirection * 4) * tick, 90);
        cameraCoord -= rightCameraDirection;
        lastMoveVector = rightCameraDirection * (-1);
    }

    //key = NULL;

#ifdef DEBUG
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { SCREEN_WIDTH + 2, 3 });
    std::cout << "                          ";
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { SCREEN_WIDTH + 2, 3 });
    std::cout << HorAngleOnNewSurface;
#endif
}

void ACharacterCamera::CamRotateOnZAxis(float angle)
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

void ACharacterCamera::CamRotateOnXYAxis(float angle)
{
    horCameraDirection = GetVectorRotateXYOnAngle(horCameraDirection, angle);
    verCameraDirection = { GetVectorRotateXYOnAngle(verCameraDirection, angle).x, GetVectorRotateXYOnAngle(verCameraDirection, angle).y, GetVectorRotateXYOnAngle(verCameraDirection, angle).z / cos(fromDegreesToRad(angle)) };

    CalcCameraHorAngleOnNewSurface();

    bool isHorAngleOnNewSurfaceHaveBadV = (HorAngleOnNewSurface >= 359 || HorAngleOnNewSurface <= 1) || (HorAngleOnNewSurface >= 89 && HorAngleOnNewSurface <= 91) || (HorAngleOnNewSurface >= 179 && HorAngleOnNewSurface <= 181) || (HorAngleOnNewSurface >= 269 && HorAngleOnNewSurface <= 271);
    //while (HorAngleOnNewSurface == HorAngleOnNewSurfaceErrorValue || isHorAngleOnNewSurfaceHaveBadV)
    while (HorAngleOnNewSurface == HorAngleOnNewSurfaceErrorValue)
    {
        horCameraDirection = GetVectorRotateXYOnAngle(horCameraDirection, angle);
        verCameraDirection = { GetVectorRotateXYOnAngle(verCameraDirection, angle).x, GetVectorRotateXYOnAngle(verCameraDirection, angle).y, GetVectorRotateXYOnAngle(verCameraDirection, angle).z / cos(fromDegreesToRad(angle)) };
        CalcCameraHorAngleOnNewSurface();

        isHorAngleOnNewSurfaceHaveBadV = (HorAngleOnNewSurface >= 359 || HorAngleOnNewSurface <= 1) || (HorAngleOnNewSurface >= 89 && HorAngleOnNewSurface <= 91) || (HorAngleOnNewSurface >= 179 && HorAngleOnNewSurface <= 181) || (HorAngleOnNewSurface >= 269 && HorAngleOnNewSurface <= 271);
    }
}