#pragma once

#include "Maths.h"

class Light
{
private:
    int IValue;
    COORDS lightCoord;

public:
    Light(int IValue, COORDS lightCoord) noexcept;

    float GetLightPower(COORDS dotCoords) const;

    COORDS GetCoord() const noexcept;

    COORDS& SetCoord() noexcept;
};