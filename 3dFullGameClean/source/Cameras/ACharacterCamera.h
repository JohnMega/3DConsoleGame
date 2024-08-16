#pragma once

#include <Windows.h>
#include "ACamera.h"

extern char key;
extern POINT NewMouseCoords;
extern POINT OldMouseCoords;

class ACharacterCamera : public ACamera
{
    static const char LEFT = 'a';
    static const char RIGHT = 'd';
    static const char FORWARD = 'w';
    static const char BACK = 's';

private:
    void CamRotateOnZAxis(float angle);

    void CamRotateOnXYAxis(float angle);

public:
    using ACamera::ACamera;

    void Move() noexcept override;
};