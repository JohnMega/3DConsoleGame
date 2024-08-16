#pragma once

#include "Maths.h"

class ACamera
{
    static inline const COORDS StartHorCameraDirection = { 1, 0.01, 0 };
    static inline const COORDS StartVerCameraDirection = { 1, 0.01, 0.05 };

protected:
    static const int HorAngleOnNewSurfaceErrorValue = -1;

protected:
    float HorAngleOnNewSurface;
    COORDS cameraCoord;
    COORDS cameraCoordForRender;
    COORDS horCameraDirection;
    COORDS verCameraDirection;
    COORDS rightCameraDirection;
    COORDS lastMoveVector;

public:
    ACamera();

    explicit ACamera(COORDS cameraCoord);

    virtual void Move() {};

    void CalcCameraHorAngleOnNewSurface();

    COORDS GetVerCamDir() const noexcept;

    COORDS& SetVerCamDir() noexcept;

    COORDS GetHorCamDir() const noexcept;

    COORDS& SetHorCamDir() noexcept;

    COORDS GetCameraCoord() const noexcept;

    COORDS& SetCameraCoord() noexcept;

    COORDS GetCameraCoordForRender() const noexcept;

    COORDS& SetCameraCoordForRender() noexcept;

    COORDS& SetLastMoveVector() noexcept;

    COORDS GetLastMoveVector() const noexcept;

    float GetCameraHorAngleOnNewSurface() const noexcept;
};