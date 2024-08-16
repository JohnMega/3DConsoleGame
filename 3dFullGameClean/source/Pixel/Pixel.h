#pragma once

#include "Maths.h"

class ACamera;

class Pixel
{
private:
    float ScreenPosX;
    float ScreenPosY;

private:
    void ScreenPosXCalc(const ACamera&, COORDS, COORDS);

    void ScreenPosYCalc(const ACamera&);

    void ScreenPosXCalcPart(const ACamera&);

    void ScreenPosYCalcPart(const ACamera&);

protected:
    COORDS dotCoord;

public:
    Pixel() noexcept;

    Pixel(COORDS dotCoord) noexcept;

    float GetScreenPosX() const noexcept;

    float GetScreenPosY() const noexcept;

    COORDS& SetDotCoord() noexcept;

    void CalcDotCoordsOnScreen(const ACamera&);
};