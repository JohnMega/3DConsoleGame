#pragma once

#include "Parallelepiped.h"
#include "Maths.h"

class SkyboxPart : public Parallelepiped
{
public:
    SkyboxPart(float length, float width, float height, COORDS centreCoord, int color) noexcept;

    void Rotation(float angle, size_t flag = 0) noexcept override;

    COORDS GetParalSize() const noexcept;

    std::string GetTextureName() const noexcept;

    COORDS GetCentreCoord() const noexcept override;

    COORDS& SetCentreCoord() noexcept override;

    Range GetRange() const noexcept override;

    Range& SetRange() noexcept override;

    void UpdateRange(size_t flag = 0) noexcept override;

    objectType GetObjType() const noexcept override;

    bool IsRotate() const noexcept override;

    bool IsParaboloid() const noexcept override;

    bool ToParab(COORDS cameraNormalizeRay, COORDS cameraCoord, COORDS& nearCrossingRayAndFigureCoord, float farCrossingRayAndFigureValue) noexcept override;

    int GetColor() const noexcept override;

    COORDS GetColor(COORDS paralDot) const override;

    bool isObjHasTexture() const noexcept override;

    void SetObjTextureParams(const std::string& textureName) noexcept;

    void SetObjTextureMode(bool) noexcept;

    int& SetColor() noexcept override;
};