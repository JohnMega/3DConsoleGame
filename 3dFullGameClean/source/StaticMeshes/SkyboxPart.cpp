
#include "SkyboxPart.h"

SkyboxPart::SkyboxPart(float length, float width, float height, COORDS centreCoord, int color) noexcept
    : Parallelepiped(length, width, height, centreCoord, false, false, false, color)
{
    SetParRange();
    SetGroundParMainDots(groundParMainDot1, groundParMainDot2, groundParMainDot3, groundParMainDot4);
    SetExtendedGroundParMainDots();

    objType = objectType::SKYBOX;
}

void SkyboxPart::Rotation(float angle, size_t flag) noexcept
{ }

COORDS SkyboxPart::GetParalSize() const noexcept
{
    return Parallelepiped::GetParalSize();
}

std::string SkyboxPart::GetTextureName() const noexcept
{
    return Parallelepiped::GetTextureName();
}

COORDS SkyboxPart::GetCentreCoord() const noexcept
{
    return Parallelepiped::GetCentreCoord();
}

COORDS& SkyboxPart::SetCentreCoord() noexcept
{
    return Parallelepiped::SetCentreCoord();
}

Range SkyboxPart::GetRange() const noexcept
{
    return Parallelepiped::GetRange();
}

Range& SkyboxPart::SetRange() noexcept
{
    return Parallelepiped::SetRange();
}

void SkyboxPart::UpdateRange(size_t flag) noexcept
{
    Parallelepiped::UpdateRange(flag);
}

objectType SkyboxPart::GetObjType() const noexcept
{
    return Parallelepiped::GetObjType();
}

bool SkyboxPart::IsRotate() const noexcept
{
    return false;
}

bool SkyboxPart::IsParaboloid() const noexcept
{
    return false;
}

bool SkyboxPart::ToParab(COORDS cameraNormalizeRay, COORDS cameraCoord, COORDS& nearCrossingRayAndFigureCoord, float farCrossingRayAndFigureValue) noexcept
{
    return false;
}

int SkyboxPart::GetColor() const noexcept
{
    return Parallelepiped::GetColor();
}

COORDS SkyboxPart::GetColor(COORDS paralDot) const
{
    return Parallelepiped::GetColor(paralDot);
}

bool SkyboxPart::isObjHasTexture() const noexcept
{
    return Parallelepiped::isObjHasTexture();
}

void SkyboxPart::SetObjTextureParams(const std::string& textureName) noexcept
{
    Parallelepiped::SetObjTextureParams(textureName, 1, 1);
}

void SkyboxPart::SetObjTextureMode(bool objTextureMode) noexcept
{
    Parallelepiped::SetObjTextureMode(objTextureMode);
}

int& SkyboxPart::SetColor() noexcept
{
    return Parallelepiped::SetColor();
}