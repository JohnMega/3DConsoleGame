#pragma once

#include "StaticMesh.h"
#include "Collision.h"
#include "Maths.h"

class Pyramid : public StaticMesh, public Collision
{
    static const int PyrBaseDotsCount = 4;
    static const int PyrInclineSurfacesCount = 4;
    static const int PyrSurfacesCount = 6;
    static inline const float extendValue = 0.01F;

private:
    float lowerBaselength, lowerBasewidth, height; // Dont move it
    float upperBaseLength, upperBaseWidth; // Dont move it
    int color; // Dont move it
    COORDS centreCoord;
    COORDS lowerBasePyrMainDot1, lowerBasePyrMainDot2, lowerBasePyrMainDot3, lowerBasePyrMainDot4;
    COORDS extendedLowerBasePyrMainDot1, extendedLowerBasePyrMainDot2, extendedLowerBasePyrMainDot3, extendedLowerBasePyrMainDot4;
    COORDS upperBasePyrMainDot1, upperBasePyrMainDot2, upperBasePyrMainDot3, upperBasePyrMainDot4;
    COORDS extendedUpperBasePyrMainDot1, extendedUpperBasePyrMainDot2, extendedUpperBasePyrMainDot3, extendedUpperBasePyrMainDot4;
    Range basePyrRange, upperBasePyrRange;

private:
    void SetPyrRanges() noexcept;

    void SetBasesPyrMainDots() noexcept;

    void SetExtendedBasesPyrMainDots() noexcept;

    bool IsDotDisplayAfterFiguresDel(COORDS cameraNormalizeRay, COORDS cameraCoord, COORDS& nearCrossingRayAndFigureCoord, float farCrossingRayAndFigureValue, int& rayHitSurfaceNumber) const noexcept;
    void SurfacesValuesCalc(COORDS checkedDot, COORDS unnecessaryShapeMainDot1, COORDS unnecessaryShapeMainDot2, COORDS unnecessaryShapeMainDot3, COORDS unnecessaryShapeMainDot4, COORDS unnecessaryShapeMainDot5, COORDS unnecessaryShapeMainDot6, float& surfaceValue1, float& reverseSurfaceValue1, float& surfaceValue2, float& reverseSurfaceValue2, float& surfaceValue3, float& reverseSurfaceValue3, float& surfaceValue4, float& reverseSurfaceValue4, float& surfaceValue5, float& reverseSurfaceValue5) const noexcept;
    COORDS ExtendedNearCrossingRayAndFigureCoordCalc(COORDS unnecessaryShapeMainDot1, COORDS unnecessaryShapeMainDot2, COORDS unnecessaryShapeMainDot3, COORDS nearCrossingRayAndFigureCoord) const;

    bool IsRotatedFigureShine(COORDS mainDot1, COORDS mainDot2, int rayHitSurfaceNumber);

    bool IsRayCrossRotatedFigure(COORDS startRayCoord, COORDS endRayCoord) const noexcept;

    bool IsDotInRotatedFigure(COORDS dotCoord) const noexcept;

    COORDS DownDotCollisionHandle(COORDS mainDot, COORDS finalSurfaceDot1, COORDS finalSurfaceDot2, COORDS finalSurfaceDot3) override;

public:
    Pyramid(float lowerBaselength, float lowerBasewidth, float height, float upperBaseLength, float upperBaseWidth, COORDS centreCoord, int color);

    COORDS CollisionHandle(COORDS mainDotDir, COORDS mainDot) override;

    bool IsRotate() const noexcept override;

    COORDS GetCentreCoord() const noexcept override;

    COORDS& SetCentreCoord() noexcept;

    Range GetRange() const noexcept override;

    objectType GetObjType() const noexcept override;

    void UpdateRange(size_t flag = 0) noexcept override;

    int GetColor() const noexcept override;

    int& SetColor() noexcept override;
};