#pragma once

#include <Windows.h>
#include "Maths.h"
#include "StaticMesh.h"
#include "ACamera.h"

extern char key;
extern POINT NewMouseCoords;
extern POINT OldMouseCoords;

class ASphereCamera : public ACamera
{
    StaticMesh* observedObject;

    // Motion Cubic
    float Position;
    float Rate;

public:
    ASphereCamera(COORDS cameraCoord, StaticMesh* observedObject);

    void Move() noexcept override;

    void SetObservedObject(StaticMesh* observedObject) noexcept;
};