
#include <cmath>
#include "Light.h"

Light::Light(int IValue, COORDS lightCoord) noexcept : IValue(IValue), lightCoord(lightCoord)
{ }

float Light::GetLightPower(COORDS dotCoords) const
{
    return (IValue / (pow(dotDistance(lightCoord, dotCoords), 2)));
}

COORDS Light::GetCoord() const noexcept
{
    return lightCoord;
}

COORDS& Light::SetCoord() noexcept
{
    return lightCoord;
}