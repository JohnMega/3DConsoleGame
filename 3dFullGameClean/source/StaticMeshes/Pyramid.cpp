
#include <cmath>
#include <Windows.h>
#include "Pyramid.h"

Pyramid::Pyramid(float lowerBaselength, float lowerBasewidth, float height, float upperBaseLength, float upperBaseWidth, COORDS centreCoord, int color)
    : lowerBaselength(lowerBaselength)
    , lowerBasewidth(lowerBasewidth)
    , height(height)
    , upperBaseLength(upperBaseLength)
    , upperBaseWidth(upperBaseWidth)
    , centreCoord(centreCoord)
    , color(color)
{
    SetPyrRanges();
    SetBasesPyrMainDots();
    SetExtendedBasesPyrMainDots();

    objType = objectType::PYRAMID;
}

void Pyramid::UpdateRange(size_t flag) noexcept
{
    SetPyrRanges();
    SetBasesPyrMainDots();
    SetExtendedBasesPyrMainDots();
}

bool Pyramid::IsRotate() const noexcept
{
    return true;
}

COORDS Pyramid::DownDotCollisionHandle(COORDS mainDot, COORDS finalSurfaceDot1, COORDS finalSurfaceDot2, COORDS finalSurfaceDot3)
{
    COORDS surfaceAndNormCross = GetCrossingSurfaceAndLineCoords(finalSurfaceDot1, finalSurfaceDot2, finalSurfaceDot3, mainDot - centreCoord, (mainDot - centreCoord) + COORDS{ 0,0,1 });

    return (COORDS{ 0,0,1 } *(dotDistance(surfaceAndNormCross, mainDot - centreCoord) + 0.01));
}

COORDS Pyramid::CollisionHandle(COORDS mainDotDir, COORDS mainDot)
{
    float surfaceValue1 = GetSurfaceValue(extendedLowerBasePyrMainDot1, extendedLowerBasePyrMainDot2, extendedUpperBasePyrMainDot1, mainDot - centreCoord);
    float reverseSurfaceValue1 = GetSurfaceValue(extendedLowerBasePyrMainDot1, extendedLowerBasePyrMainDot2, extendedUpperBasePyrMainDot1, extendedLowerBasePyrMainDot3);
    float surfaceValue2 = GetSurfaceValue(extendedLowerBasePyrMainDot2, extendedLowerBasePyrMainDot3, extendedUpperBasePyrMainDot2, mainDot - centreCoord);
    float reverseSurfaceValue2 = GetSurfaceValue(extendedLowerBasePyrMainDot2, extendedLowerBasePyrMainDot3, extendedUpperBasePyrMainDot2, extendedLowerBasePyrMainDot1);
    float surfaceValue3 = GetSurfaceValue(extendedLowerBasePyrMainDot3, extendedLowerBasePyrMainDot4, extendedUpperBasePyrMainDot3, mainDot - centreCoord);
    float reverseSurfaceValue3 = GetSurfaceValue(extendedLowerBasePyrMainDot3, extendedLowerBasePyrMainDot4, extendedUpperBasePyrMainDot3, extendedLowerBasePyrMainDot1);
    float surfaceValue4 = GetSurfaceValue(extendedLowerBasePyrMainDot4, extendedLowerBasePyrMainDot1, extendedUpperBasePyrMainDot4, mainDot - centreCoord);
    float reverseSurfaceValue4 = GetSurfaceValue(extendedLowerBasePyrMainDot4, extendedLowerBasePyrMainDot1, extendedUpperBasePyrMainDot4, extendedLowerBasePyrMainDot2);
    float surfaceValue5 = GetSurfaceValue(extendedLowerBasePyrMainDot1, extendedLowerBasePyrMainDot2, extendedLowerBasePyrMainDot3, mainDot - centreCoord);
    float reverseSurfaceValue5 = GetSurfaceValue(extendedLowerBasePyrMainDot1, extendedLowerBasePyrMainDot2, extendedLowerBasePyrMainDot3, extendedUpperBasePyrMainDot4);
    float surfaceValue6 = GetSurfaceValue(extendedUpperBasePyrMainDot1, extendedUpperBasePyrMainDot2, extendedUpperBasePyrMainDot3, mainDot - centreCoord);
    float reverseSurfaceValue6 = GetSurfaceValue(extendedUpperBasePyrMainDot1, extendedUpperBasePyrMainDot2, extendedUpperBasePyrMainDot3, extendedLowerBasePyrMainDot1);

    if ((IsSign(surfaceValue1, reverseSurfaceValue1) && IsSign(surfaceValue2, reverseSurfaceValue2) && IsSign(surfaceValue3, reverseSurfaceValue3) && IsSign(surfaceValue4, reverseSurfaceValue4) && IsSign(surfaceValue5, reverseSurfaceValue5)) && IsSign(surfaceValue6, reverseSurfaceValue6))
    {
        COORDS mainPyrDots[PyrBaseDotsCount] = { extendedLowerBasePyrMainDot1, extendedLowerBasePyrMainDot2, extendedLowerBasePyrMainDot3, extendedLowerBasePyrMainDot4 };
        COORDS mainPyrDotsUp[PyrBaseDotsCount] = { extendedUpperBasePyrMainDot1, extendedUpperBasePyrMainDot2, extendedUpperBasePyrMainDot3, extendedUpperBasePyrMainDot4 };
        COORDS surfaceDot1, surfaceDot2, surfaceDot3, surfaceDot4;
        COORDS normVec, dotProjOnSurface;;

        float minDist = 10000;
        COORDS finalSurfaceDot1, finalSurfaceDot2, finalSurfaceDot3;
        COORDS finalDotProjOnSurface;
        for (size_t index = 0; index < PyrSurfacesCount; ++index)
        {
            if (index <= 3)
            {
                surfaceDot1 = mainPyrDots[index];
                surfaceDot2 = mainPyrDots[(index == 3) ? 0 : index + 1];
                surfaceDot3 = mainPyrDotsUp[index];
                surfaceDot4 = mainPyrDotsUp[(index == 3) ? 0 : index + 1];
            }

            if (index == 4)
            {
                surfaceDot1 = extendedLowerBasePyrMainDot1;
                surfaceDot2 = extendedLowerBasePyrMainDot2;
                surfaceDot3 = extendedLowerBasePyrMainDot3;
                surfaceDot4 = extendedLowerBasePyrMainDot4;
            }

            if (index == 5)
            {
                surfaceDot1 = extendedUpperBasePyrMainDot1;
                surfaceDot2 = extendedUpperBasePyrMainDot2;
                surfaceDot3 = extendedUpperBasePyrMainDot3;
                surfaceDot4 = extendedUpperBasePyrMainDot4;
            }

            float len1 = IsDotCrossTriangle(surfaceDot1, surfaceDot2, surfaceDot3, (mainDot - centreCoord) - mainDotDir * 25, mainDotDir, 100);
            float len2 = IsDotCrossTriangle(index > 3 ? surfaceDot1 : surfaceDot2, surfaceDot3, surfaceDot4, (mainDot - centreCoord) - mainDotDir * 25, mainDotDir, 100);
            if (len1 <= 0 && len2 <= 0) continue;

            dotProjOnSurface = mainDotDir * (len1 > 0 ? len1 : len2) + ((mainDot - centreCoord) - mainDotDir * 25);

            if ((len1 > 0 || len2 > 0) && dotDistance((mainDot - centreCoord) - mainDotDir * 25, dotProjOnSurface) < minDist)
            {
                finalSurfaceDot1 = surfaceDot1;
                finalSurfaceDot2 = surfaceDot2;
                finalSurfaceDot3 = surfaceDot3;
                finalDotProjOnSurface = dotProjOnSurface;
                minDist = dotDistance((mainDot - centreCoord) - mainDotDir * 25, dotProjOnSurface);
            }
        }

        if (mainDotDir.x == 0 && mainDotDir.y == 0)
        {
            return DownDotCollisionHandle(mainDot, finalSurfaceDot1, finalSurfaceDot2, finalSurfaceDot3);
        }

        normVec = vectorNormalize(GetSurfaceNormVec(finalSurfaceDot1, finalSurfaceDot2, finalSurfaceDot3), { 0,0,0 });

        if (dotDistance(COORDS{ normVec.x / 100, normVec.y / 100, normVec.z / 100 } + finalDotProjOnSurface, (mainDot - centreCoord)) < dotDistance((COORDS{ normVec.x / 100, normVec.y / 100, normVec.z / 100 } *(-1)) + finalDotProjOnSurface, (mainDot - centreCoord)))
            normVec = normVec * (-1);

        COORDS surfaceAndNormCross = GetCrossingSurfaceAndLineCoords(finalSurfaceDot1, finalSurfaceDot2, finalSurfaceDot3, mainDot - centreCoord, (mainDot - centreCoord) + normVec);

        return (normVec * (dotDistance(surfaceAndNormCross, mainDot - centreCoord) + 0.01));
    }

    return { 0,0,0 };
}

COORDS Pyramid::GetCentreCoord() const noexcept
{
    return centreCoord;
}

COORDS& Pyramid::SetCentreCoord() noexcept
{
    SetPyrRanges();
    return centreCoord;
}

Range Pyramid::GetRange() const noexcept
{
    return basePyrRange;
}

objectType Pyramid::GetObjType() const noexcept
{
    return objType;
}

int Pyramid::GetColor() const noexcept
{
    return color;
}

int& Pyramid::SetColor() noexcept
{
    return color;
}

COORDS Pyramid::ExtendedNearCrossingRayAndFigureCoordCalc(COORDS unnecessaryShapeMainDot1, COORDS unnecessaryShapeMainDot2, COORDS unnecessaryShapeMainDot3, COORDS nearCrossingRayAndFigureCoord) const
{
    float surfaceNormVecX, surfaceNormVecY, surfaceNormVecZ;

    surfaceNormVecX = (unnecessaryShapeMainDot2.y - unnecessaryShapeMainDot1.y) * (unnecessaryShapeMainDot3.z - unnecessaryShapeMainDot1.z) - (unnecessaryShapeMainDot3.y - unnecessaryShapeMainDot1.y) * (unnecessaryShapeMainDot2.z - unnecessaryShapeMainDot1.z);
    surfaceNormVecY = (unnecessaryShapeMainDot2.z - unnecessaryShapeMainDot1.z) * (unnecessaryShapeMainDot3.x - unnecessaryShapeMainDot1.x) - (unnecessaryShapeMainDot3.z - unnecessaryShapeMainDot1.z) * (unnecessaryShapeMainDot2.x - unnecessaryShapeMainDot1.x);
    surfaceNormVecZ = (unnecessaryShapeMainDot2.x - unnecessaryShapeMainDot1.x) * (unnecessaryShapeMainDot3.y - unnecessaryShapeMainDot1.y) - (unnecessaryShapeMainDot3.x - unnecessaryShapeMainDot1.x) * (unnecessaryShapeMainDot2.y - unnecessaryShapeMainDot1.y);

    COORDS surfaceNormVec = { surfaceNormVecX, surfaceNormVecY, surfaceNormVecZ };

    surfaceNormVec = { surfaceNormVecX / 100, surfaceNormVecY / 100, surfaceNormVecZ / 100 };

    COORDS checkDot1 = { nearCrossingRayAndFigureCoord.x - centreCoord.x + surfaceNormVec.x, nearCrossingRayAndFigureCoord.y - centreCoord.y + surfaceNormVec.y, nearCrossingRayAndFigureCoord.z - centreCoord.z + surfaceNormVec.z };

    surfaceNormVec = { -surfaceNormVecX / 100, -surfaceNormVecY / 100, -surfaceNormVecZ / 100 };

    COORDS checkDot2 = { nearCrossingRayAndFigureCoord.x - centreCoord.x + surfaceNormVec.x, nearCrossingRayAndFigureCoord.y - centreCoord.y + surfaceNormVec.y, nearCrossingRayAndFigureCoord.z - centreCoord.z + surfaceNormVec.z };

    if (dotDistance(checkDot1, { 0,0,0 }) < dotDistance(checkDot2, { 0,0,0 }))
        surfaceNormVec = { -surfaceNormVecX, -surfaceNormVecY, -surfaceNormVecZ };
    else
        surfaceNormVec = { surfaceNormVecX, surfaceNormVecY, surfaceNormVecZ };

    return COORDS{ nearCrossingRayAndFigureCoord.x + surfaceNormVec.x * 0.00001F, nearCrossingRayAndFigureCoord.y + surfaceNormVec.y * 0.00001F, nearCrossingRayAndFigureCoord.z + surfaceNormVec.z * 0.00001F };
}

void Pyramid::SetPyrRanges() noexcept
{
    basePyrRange.xStart = centreCoord.x - (lowerBaselength / 2);
    basePyrRange.xEnd = centreCoord.x + (lowerBaselength / 2);
    basePyrRange.yStart = centreCoord.y - (lowerBasewidth / 2);
    basePyrRange.yEnd = centreCoord.y + (lowerBasewidth / 2);
    basePyrRange.zStart = centreCoord.z - (height / 2);
    basePyrRange.zEnd = centreCoord.z + (height / 2);

    upperBasePyrRange.xStart = centreCoord.x - (upperBaseLength / 2);
    upperBasePyrRange.xEnd = centreCoord.x + (upperBaseLength / 2);
    upperBasePyrRange.yStart = centreCoord.y - (upperBaseWidth / 2);
    upperBasePyrRange.yEnd = centreCoord.y + (upperBaseWidth / 2);
    upperBasePyrRange.zStart = 0;
    upperBasePyrRange.zEnd = 0;
}

void Pyramid::SetBasesPyrMainDots() noexcept
{
    lowerBasePyrMainDot1 = { basePyrRange.xStart - centreCoord.x, basePyrRange.yStart - centreCoord.y, basePyrRange.zStart - centreCoord.z };
    lowerBasePyrMainDot2 = { basePyrRange.xEnd - centreCoord.x, basePyrRange.yStart - centreCoord.y, basePyrRange.zStart - centreCoord.z };
    lowerBasePyrMainDot3 = { basePyrRange.xEnd - centreCoord.x, basePyrRange.yEnd - centreCoord.y, basePyrRange.zStart - centreCoord.z };
    lowerBasePyrMainDot4 = { basePyrRange.xStart - centreCoord.x, basePyrRange.yEnd - centreCoord.y, basePyrRange.zStart - centreCoord.z };

    upperBasePyrMainDot1 = { upperBasePyrRange.xStart - centreCoord.x, upperBasePyrRange.yStart - centreCoord.y, basePyrRange.zEnd - centreCoord.z };
    upperBasePyrMainDot2 = { upperBasePyrRange.xEnd - centreCoord.x, upperBasePyrRange.yStart - centreCoord.y, basePyrRange.zEnd - centreCoord.z };
    upperBasePyrMainDot3 = { upperBasePyrRange.xEnd - centreCoord.x, upperBasePyrRange.yEnd - centreCoord.y, basePyrRange.zEnd - centreCoord.z };
    upperBasePyrMainDot4 = { upperBasePyrRange.xStart - centreCoord.x, upperBasePyrRange.yEnd - centreCoord.y, basePyrRange.zEnd - centreCoord.z };
}

void Pyramid::SetExtendedBasesPyrMainDots() noexcept
{
    extendedLowerBasePyrMainDot1 = { lowerBasePyrMainDot1.x - extendValue, lowerBasePyrMainDot1.y - extendValue, lowerBasePyrMainDot1.z };
    extendedLowerBasePyrMainDot2 = { lowerBasePyrMainDot2.x + extendValue, lowerBasePyrMainDot2.y - extendValue, lowerBasePyrMainDot2.z };
    extendedLowerBasePyrMainDot3 = { lowerBasePyrMainDot3.x + extendValue, lowerBasePyrMainDot3.y + extendValue, lowerBasePyrMainDot3.z };
    extendedLowerBasePyrMainDot4 = { lowerBasePyrMainDot4.x - extendValue, lowerBasePyrMainDot4.y + extendValue, lowerBasePyrMainDot4.z };

    extendedUpperBasePyrMainDot1 = { upperBasePyrMainDot1.x - extendValue, upperBasePyrMainDot1.y - extendValue, upperBasePyrMainDot1.z + extendValue };
    extendedUpperBasePyrMainDot2 = { upperBasePyrMainDot2.x + extendValue, upperBasePyrMainDot2.y - extendValue, upperBasePyrMainDot2.z + extendValue };
    extendedUpperBasePyrMainDot3 = { upperBasePyrMainDot3.x + extendValue, upperBasePyrMainDot3.y + extendValue, upperBasePyrMainDot3.z + extendValue };
    extendedUpperBasePyrMainDot4 = { upperBasePyrMainDot4.x - extendValue, upperBasePyrMainDot4.y + extendValue, upperBasePyrMainDot4.z + extendValue };
}

void Pyramid::SurfacesValuesCalc(COORDS checkedDot, COORDS unnecessaryShapeMainDot1, COORDS unnecessaryShapeMainDot2, COORDS unnecessaryShapeMainDot3, COORDS unnecessaryShapeMainDot4, COORDS unnecessaryShapeMainDot5, COORDS unnecessaryShapeMainDot6,
    float& surfaceValue1, float& reverseSurfaceValue1, float& surfaceValue2, float& reverseSurfaceValue2, float& surfaceValue3, float& reverseSurfaceValue3, float& surfaceValue4, float& reverseSurfaceValue4, float& surfaceValue5, float& reverseSurfaceValue5) const noexcept
{
    surfaceValue1 = GetSurfaceValue(unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot5, checkedDot - centreCoord);
    reverseSurfaceValue1 = GetSurfaceValue(unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot5, unnecessaryShapeMainDot3);
    surfaceValue2 = GetSurfaceValue(unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, checkedDot - centreCoord);
    reverseSurfaceValue2 = GetSurfaceValue(unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, unnecessaryShapeMainDot5);
    surfaceValue3 = GetSurfaceValue(unnecessaryShapeMainDot3, unnecessaryShapeMainDot4, unnecessaryShapeMainDot5, checkedDot - centreCoord);
    reverseSurfaceValue3 = GetSurfaceValue(unnecessaryShapeMainDot3, unnecessaryShapeMainDot4, unnecessaryShapeMainDot5, unnecessaryShapeMainDot1);
    surfaceValue4 = GetSurfaceValue(unnecessaryShapeMainDot1, unnecessaryShapeMainDot3, unnecessaryShapeMainDot5, checkedDot - centreCoord);
    reverseSurfaceValue4 = GetSurfaceValue(unnecessaryShapeMainDot1, unnecessaryShapeMainDot3, unnecessaryShapeMainDot5, unnecessaryShapeMainDot2);
    surfaceValue5 = GetSurfaceValue(unnecessaryShapeMainDot2, unnecessaryShapeMainDot4, unnecessaryShapeMainDot6, checkedDot - centreCoord);
    reverseSurfaceValue5 = GetSurfaceValue(unnecessaryShapeMainDot2, unnecessaryShapeMainDot4, unnecessaryShapeMainDot6, unnecessaryShapeMainDot1);
}

bool Pyramid::IsDotInRotatedFigure(COORDS checkedDotCoord) const noexcept
{
    float surfaceValue1, reverseSurfaceValue1, surfaceValue2, reverseSurfaceValue2, surfaceValue3, reverseSurfaceValue3, surfaceValue4,
        reverseSurfaceValue4, surfaceValue5, reverseSurfaceValue5, surfaceValue6, reverseSurfaceValue6;

    surfaceValue1 = GetSurfaceValue(extendedLowerBasePyrMainDot1, extendedLowerBasePyrMainDot2, extendedUpperBasePyrMainDot1, checkedDotCoord);
    reverseSurfaceValue1 = GetSurfaceValue(extendedLowerBasePyrMainDot1, extendedLowerBasePyrMainDot2, extendedUpperBasePyrMainDot1, extendedLowerBasePyrMainDot3);
    surfaceValue2 = GetSurfaceValue(extendedLowerBasePyrMainDot2, extendedLowerBasePyrMainDot3, extendedUpperBasePyrMainDot2, checkedDotCoord);
    reverseSurfaceValue2 = GetSurfaceValue(extendedLowerBasePyrMainDot2, extendedLowerBasePyrMainDot3, extendedUpperBasePyrMainDot2, extendedLowerBasePyrMainDot4);
    surfaceValue3 = GetSurfaceValue(extendedLowerBasePyrMainDot3, extendedLowerBasePyrMainDot4, extendedUpperBasePyrMainDot3, checkedDotCoord);
    reverseSurfaceValue3 = GetSurfaceValue(extendedLowerBasePyrMainDot3, extendedLowerBasePyrMainDot4, extendedUpperBasePyrMainDot3, extendedLowerBasePyrMainDot1);
    surfaceValue4 = GetSurfaceValue(extendedLowerBasePyrMainDot4, extendedLowerBasePyrMainDot1, extendedUpperBasePyrMainDot4, checkedDotCoord);
    reverseSurfaceValue4 = GetSurfaceValue(extendedLowerBasePyrMainDot4, extendedLowerBasePyrMainDot1, extendedUpperBasePyrMainDot4, extendedLowerBasePyrMainDot2);
    surfaceValue5 = GetSurfaceValue(extendedLowerBasePyrMainDot1, extendedLowerBasePyrMainDot2, extendedLowerBasePyrMainDot3, checkedDotCoord);
    reverseSurfaceValue5 = GetSurfaceValue(extendedLowerBasePyrMainDot1, extendedLowerBasePyrMainDot2, extendedLowerBasePyrMainDot3, extendedUpperBasePyrMainDot1);
    surfaceValue6 = GetSurfaceValue(extendedUpperBasePyrMainDot1, extendedUpperBasePyrMainDot2, extendedUpperBasePyrMainDot3, checkedDotCoord);
    reverseSurfaceValue6 = GetSurfaceValue(extendedUpperBasePyrMainDot1, extendedUpperBasePyrMainDot2, extendedUpperBasePyrMainDot3, extendedLowerBasePyrMainDot1);

    if (IsSign(surfaceValue1, reverseSurfaceValue1) && IsSign(surfaceValue2, reverseSurfaceValue2)
        && IsSign(surfaceValue3, reverseSurfaceValue3) && IsSign(surfaceValue4, reverseSurfaceValue4)
        && IsSign(surfaceValue5, reverseSurfaceValue5) && IsSign(surfaceValue6, reverseSurfaceValue6))
        return true;

    return false;
}

bool Pyramid::IsRayCrossRotatedFigure(COORDS startRayCoord, COORDS endRayCoord) const noexcept
{
    static const int PyrLowerSurfaceIndex = 4;
    static const int PyrUpperSurfaceIndex = 5;

    COORDS lowerBasePyrMainDots[PyrBaseDotsCount] = { lowerBasePyrMainDot1, lowerBasePyrMainDot2, lowerBasePyrMainDot3, lowerBasePyrMainDot4 };
    COORDS upperBasePyrMainDots[PyrBaseDotsCount] = { upperBasePyrMainDot1, upperBasePyrMainDot2, upperBasePyrMainDot3, upperBasePyrMainDot4 };
    COORDS surfaceDot1, surfaceDot2, surfaceDot3;

    for (size_t index = 0; index < 4; ++index)
    {
        if (index < PyrInclineSurfacesCount)
        {
            surfaceDot1 = lowerBasePyrMainDots[index];
            surfaceDot2 = lowerBasePyrMainDots[(index == 3) ? 0 : index + 1];
            surfaceDot3 = upperBasePyrMainDots[index];
        }

        COORDS crossingSurfaceAndLineCoords = GetCrossingSurfaceAndLineCoords(surfaceDot1, surfaceDot2, surfaceDot3, endRayCoord - centreCoord, startRayCoord - centreCoord);

        if (crossingSurfaceAndLineCoords.x + centreCoord.x >= min(startRayCoord.x, endRayCoord.x) && crossingSurfaceAndLineCoords.x + centreCoord.x <= max(startRayCoord.x, endRayCoord.x)
            && crossingSurfaceAndLineCoords.y + centreCoord.y >= min(startRayCoord.y, endRayCoord.y) && crossingSurfaceAndLineCoords.y + centreCoord.y <= max(startRayCoord.y, endRayCoord.y)
            && crossingSurfaceAndLineCoords.z + centreCoord.z >= min(startRayCoord.z, endRayCoord.z) && crossingSurfaceAndLineCoords.z + centreCoord.z <= max(startRayCoord.z, endRayCoord.z))
            if (IsDotInRotatedFigure(crossingSurfaceAndLineCoords))
                return true;
    }

    for (int index = 4; index <= 5; ++index)
    {
        if (index == PyrLowerSurfaceIndex)
        {
            surfaceDot1 = extendedLowerBasePyrMainDot1;
            surfaceDot2 = extendedLowerBasePyrMainDot2;
            surfaceDot3 = extendedLowerBasePyrMainDot3;
        }
        else if (index == PyrUpperSurfaceIndex)
        {
            surfaceDot1 = extendedUpperBasePyrMainDot1;
            surfaceDot2 = extendedUpperBasePyrMainDot2;
            surfaceDot3 = extendedUpperBasePyrMainDot3;
        }

        COORDS crossingSurfaceAndLineCoords = GetCrossingSurfaceAndLineCoords(surfaceDot1, surfaceDot2, surfaceDot3, endRayCoord - centreCoord, startRayCoord - centreCoord);

        bool isCrossSurfaceAndLineDotOnPyramidLower = (crossingSurfaceAndLineCoords.x >= min(extendedLowerBasePyrMainDot1.x, extendedLowerBasePyrMainDot3.x) && crossingSurfaceAndLineCoords.x <= max(extendedLowerBasePyrMainDot1.x, extendedLowerBasePyrMainDot3.x)
            && crossingSurfaceAndLineCoords.y >= min(extendedLowerBasePyrMainDot1.y, extendedLowerBasePyrMainDot3.y) && crossingSurfaceAndLineCoords.y <= max(extendedLowerBasePyrMainDot1.y, extendedLowerBasePyrMainDot3.y));

        bool isCrossSurfaceAndLineDotOnPyramidUpper = (crossingSurfaceAndLineCoords.x >= min(extendedUpperBasePyrMainDot1.x, extendedUpperBasePyrMainDot3.x) && crossingSurfaceAndLineCoords.x <= max(extendedUpperBasePyrMainDot1.x, extendedUpperBasePyrMainDot3.x)
            && crossingSurfaceAndLineCoords.y >= min(extendedUpperBasePyrMainDot1.y, extendedUpperBasePyrMainDot3.y) && crossingSurfaceAndLineCoords.y <= max(extendedUpperBasePyrMainDot1.y, extendedUpperBasePyrMainDot3.y));

        if (crossingSurfaceAndLineCoords.x + centreCoord.x >= min(startRayCoord.x, endRayCoord.x) && crossingSurfaceAndLineCoords.x + centreCoord.x <= max(startRayCoord.x, endRayCoord.x)
            && crossingSurfaceAndLineCoords.y + centreCoord.y >= min(startRayCoord.y, endRayCoord.y) && crossingSurfaceAndLineCoords.y + centreCoord.y <= max(startRayCoord.y, endRayCoord.y)
            && crossingSurfaceAndLineCoords.z + centreCoord.z >= min(startRayCoord.z, endRayCoord.z) && crossingSurfaceAndLineCoords.z + centreCoord.z <= max(startRayCoord.z, endRayCoord.z))
        {
            if (dotDistance(startRayCoord, endRayCoord) > dotDistance(startRayCoord, crossingSurfaceAndLineCoords + centreCoord))
                if (index == PyrLowerSurfaceIndex ? isCrossSurfaceAndLineDotOnPyramidLower : isCrossSurfaceAndLineDotOnPyramidUpper)
                    return true;
        }
    }

    return false;
}

bool Pyramid::IsRotatedFigureShine(COORDS lightDot, COORDS pyrSurfaceDot, int rayHitSurfaceNumber)
{
    static const int PyrLowerSurfaceIndex = 4;
    static const int PyrUpperSurfaceIndex = 5;

    COORDS extendedLowerBasePyrMainDots[PyrBaseDotsCount] = { extendedLowerBasePyrMainDot1, extendedLowerBasePyrMainDot2, extendedLowerBasePyrMainDot3, extendedLowerBasePyrMainDot4 };
    COORDS extendedUpperBasePyrMainDots[PyrBaseDotsCount] = { extendedUpperBasePyrMainDot1, extendedUpperBasePyrMainDot2, extendedUpperBasePyrMainDot3, extendedUpperBasePyrMainDot4 };
    COORDS surfaceDot1, surfaceDot2, surfaceDot3;
    size_t failedAttemptsCount = 0;

    //the case when the dot in question did move when removing unnecessary shapes
    for (size_t index = 0; index < PyrSurfacesCount; ++index)
    {
        if (index < PyrInclineSurfacesCount)
        {
            surfaceDot1 = extendedLowerBasePyrMainDots[index];
            surfaceDot2 = extendedLowerBasePyrMainDots[(index == 3) ? 0 : index + 1];
            surfaceDot3 = extendedUpperBasePyrMainDots[index];
        }

        if (index == PyrLowerSurfaceIndex)
        {
            surfaceDot1 = extendedLowerBasePyrMainDot1;
            surfaceDot2 = extendedLowerBasePyrMainDot2;
            surfaceDot3 = extendedLowerBasePyrMainDot3;
        }

        if (index == PyrUpperSurfaceIndex)
        {
            surfaceDot1 = extendedUpperBasePyrMainDot1;
            surfaceDot2 = extendedUpperBasePyrMainDot2;
            surfaceDot3 = extendedUpperBasePyrMainDot3;
        }

        if (index == rayHitSurfaceNumber)
        {
            if (IsRayHitFigureSurfaceDot(surfaceDot1, surfaceDot2, surfaceDot3, lightDot, pyrSurfaceDot, centreCoord))
                return true;
        }
        else
            failedAttemptsCount++;
    }

    //the case when the dot in question did not move when removing unnecessary shapes
    if (failedAttemptsCount == PyrSurfacesCount)
    {
        COORDS lowerBasePyrMainDots[PyrBaseDotsCount] = { lowerBasePyrMainDot1, lowerBasePyrMainDot2, lowerBasePyrMainDot3, lowerBasePyrMainDot4 };

        static const int PyrNonBaseSurfacesCount = 4;
        for (size_t index = 0; index < PyrNonBaseSurfacesCount; ++index)
        {
            surfaceDot1 = lowerBasePyrMainDots[index];
            surfaceDot2 = lowerBasePyrMainDots[(index == 3) ? 0 : index + 1];
            surfaceDot3 = { surfaceDot1.x, surfaceDot1.y, upperBasePyrMainDot1.z };

            if (abs(GetSurfaceValue(surfaceDot1, surfaceDot2, surfaceDot3, pyrSurfaceDot - centreCoord)) < extendValue)
            {
                if (IsRayHitFigureSurfaceDot(surfaceDot1, surfaceDot2, surfaceDot3, lightDot, pyrSurfaceDot, centreCoord))
                    return true;
            }
        }
    }

    return false;
}

bool Pyramid::IsDotDisplayAfterFiguresDel(COORDS cameraNormalizeRay, COORDS cameraCoord, COORDS& nearCrossingRayAndFigureCoord, float farCrossingRayAndFigureValue, int& rayHitSurfaceNumber) const noexcept
{
    COORDS farCrossingRayAndFigureCoord = { cameraNormalizeRay.x * farCrossingRayAndFigureValue + cameraCoord.x, cameraNormalizeRay.y * farCrossingRayAndFigureValue + cameraCoord.y, cameraNormalizeRay.z * farCrossingRayAndFigureValue + cameraCoord.z };
    COORDS extendedLowerBasePyrMainDots[PyrBaseDotsCount] = { extendedLowerBasePyrMainDot1, extendedLowerBasePyrMainDot2, extendedLowerBasePyrMainDot3, extendedLowerBasePyrMainDot4 };
    COORDS extendedUpperBasePyrMainDots[PyrBaseDotsCount] = { extendedUpperBasePyrMainDot1, extendedUpperBasePyrMainDot2, extendedUpperBasePyrMainDot3, extendedUpperBasePyrMainDot4 };
    COORDS unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, unnecessaryShapeMainDot4, unnecessaryShapeMainDot5, unnecessaryShapeMainDot6;
    float surfaceValue1, reverseSurfaceValue1, surfaceValue2, reverseSurfaceValue2, surfaceValue3, reverseSurfaceValue3, surfaceValue4,
        reverseSurfaceValue4, surfaceValue5, reverseSurfaceValue5;
    bool isDotDisplayAfterFiguresDel = false;

    for (size_t i = 0; i < PyrInclineSurfacesCount; ++i)
    {
        unnecessaryShapeMainDot1 = extendedLowerBasePyrMainDots[i];
        unnecessaryShapeMainDot2 = extendedLowerBasePyrMainDots[(i == 3) ? 0 : i + 1];
        unnecessaryShapeMainDot3 = extendedUpperBasePyrMainDots[i];
        unnecessaryShapeMainDot4 = extendedUpperBasePyrMainDots[(i == 3) ? 0 : i + 1];
        unnecessaryShapeMainDot5 = { unnecessaryShapeMainDot1.x, unnecessaryShapeMainDot1.y, unnecessaryShapeMainDot3.z };
        unnecessaryShapeMainDot6 = { unnecessaryShapeMainDot2.x, unnecessaryShapeMainDot2.y, unnecessaryShapeMainDot4.z };

        SurfacesValuesCalc(nearCrossingRayAndFigureCoord, unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, unnecessaryShapeMainDot4, unnecessaryShapeMainDot5, unnecessaryShapeMainDot6, surfaceValue1, reverseSurfaceValue1, surfaceValue2, reverseSurfaceValue2, surfaceValue3, reverseSurfaceValue3, surfaceValue4, reverseSurfaceValue4, surfaceValue5, reverseSurfaceValue5);

        if ((IsSign(surfaceValue1, reverseSurfaceValue1) && IsSign(surfaceValue2, reverseSurfaceValue2) && IsSign(surfaceValue3, reverseSurfaceValue3) && IsSign(surfaceValue4, reverseSurfaceValue4) && IsSign(surfaceValue5, reverseSurfaceValue5)))
        {
            SurfacesValuesCalc(farCrossingRayAndFigureCoord, unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, unnecessaryShapeMainDot4, unnecessaryShapeMainDot5, unnecessaryShapeMainDot6, surfaceValue1, reverseSurfaceValue1, surfaceValue2, reverseSurfaceValue2, surfaceValue3, reverseSurfaceValue3, surfaceValue4, reverseSurfaceValue4, surfaceValue5, reverseSurfaceValue5);

            if ((IsSign(surfaceValue1, reverseSurfaceValue1) && IsSign(surfaceValue2, reverseSurfaceValue2) && IsSign(surfaceValue3, reverseSurfaceValue3) && IsSign(surfaceValue4, reverseSurfaceValue4) && IsSign(surfaceValue5, reverseSurfaceValue5)))
                return false;

            COORDS farCrossingRayAndFigureExtendedCoord = { cameraNormalizeRay.x * (farCrossingRayAndFigureValue + 1) + cameraCoord.x, cameraNormalizeRay.y * (farCrossingRayAndFigureValue + 1) + cameraCoord.y, cameraNormalizeRay.z * (farCrossingRayAndFigureValue + 1) + cameraCoord.z };
            COORDS crossingSurfaceAndLineCoords = GetCrossingSurfaceAndLineCoords(unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, nearCrossingRayAndFigureCoord - centreCoord, farCrossingRayAndFigureExtendedCoord - centreCoord);

            nearCrossingRayAndFigureCoord = crossingSurfaceAndLineCoords + centreCoord;
            rayHitSurfaceNumber = i;

            if (!(nearCrossingRayAndFigureCoord.x > basePyrRange.xStart && nearCrossingRayAndFigureCoord.x < basePyrRange.xEnd && nearCrossingRayAndFigureCoord.y > basePyrRange.yStart && nearCrossingRayAndFigureCoord.y < basePyrRange.yEnd && nearCrossingRayAndFigureCoord.z > basePyrRange.zStart && nearCrossingRayAndFigureCoord.z < basePyrRange.zEnd))
                return false;

            for (size_t j = 0; j < PyrInclineSurfacesCount; j++)
            {
                if (i == j) continue;

                unnecessaryShapeMainDot1 = extendedLowerBasePyrMainDots[j];
                unnecessaryShapeMainDot2 = extendedLowerBasePyrMainDots[(j == 3) ? 0 : j + 1];
                unnecessaryShapeMainDot3 = extendedUpperBasePyrMainDots[j];
                unnecessaryShapeMainDot4 = extendedUpperBasePyrMainDots[(j == 3) ? 0 : j + 1];
                unnecessaryShapeMainDot5 = { unnecessaryShapeMainDot1.x, unnecessaryShapeMainDot1.y, unnecessaryShapeMainDot3.z };
                unnecessaryShapeMainDot6 = { unnecessaryShapeMainDot2.x, unnecessaryShapeMainDot2.y, unnecessaryShapeMainDot4.z };

                SurfacesValuesCalc(nearCrossingRayAndFigureCoord, unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, unnecessaryShapeMainDot4, unnecessaryShapeMainDot5, unnecessaryShapeMainDot6, surfaceValue1, reverseSurfaceValue1, surfaceValue2, reverseSurfaceValue2, surfaceValue3, reverseSurfaceValue3, surfaceValue4, reverseSurfaceValue4, surfaceValue5, reverseSurfaceValue5);

                if ((IsSign(surfaceValue1, reverseSurfaceValue1) && IsSign(surfaceValue2, reverseSurfaceValue2) && IsSign(surfaceValue3, reverseSurfaceValue3) && IsSign(surfaceValue4, reverseSurfaceValue4) && IsSign(surfaceValue5, reverseSurfaceValue5)))
                {
                    SurfacesValuesCalc(farCrossingRayAndFigureCoord, unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, unnecessaryShapeMainDot4, unnecessaryShapeMainDot5, unnecessaryShapeMainDot6, surfaceValue1, reverseSurfaceValue1, surfaceValue2, reverseSurfaceValue2, surfaceValue3, reverseSurfaceValue3, surfaceValue4, reverseSurfaceValue4, surfaceValue5, reverseSurfaceValue5);

                    if ((IsSign(surfaceValue1, reverseSurfaceValue1) && IsSign(surfaceValue2, reverseSurfaceValue2) && IsSign(surfaceValue3, reverseSurfaceValue3) && IsSign(surfaceValue4, reverseSurfaceValue4) && IsSign(surfaceValue5, reverseSurfaceValue5)))
                        return false;

                    crossingSurfaceAndLineCoords = GetCrossingSurfaceAndLineCoords(unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, nearCrossingRayAndFigureCoord - centreCoord, farCrossingRayAndFigureExtendedCoord - centreCoord);
                    nearCrossingRayAndFigureCoord = crossingSurfaceAndLineCoords + centreCoord;

                    COORDS extendedNearCrossingRayAndFigureCoord = ExtendedNearCrossingRayAndFigureCoordCalc(unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, nearCrossingRayAndFigureCoord);
                    SurfacesValuesCalc(extendedNearCrossingRayAndFigureCoord, unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, unnecessaryShapeMainDot4, unnecessaryShapeMainDot5, unnecessaryShapeMainDot6, surfaceValue1, reverseSurfaceValue1, surfaceValue2, reverseSurfaceValue2, surfaceValue3, reverseSurfaceValue3, surfaceValue4, reverseSurfaceValue4, surfaceValue5, reverseSurfaceValue5);

                    if (!(IsSign(surfaceValue1, reverseSurfaceValue1) && IsSign(surfaceValue2, reverseSurfaceValue2) && IsSign(surfaceValue3, reverseSurfaceValue3) && IsSign(surfaceValue4, reverseSurfaceValue4) && IsSign(surfaceValue5, reverseSurfaceValue5)))
                        return false;

                    rayHitSurfaceNumber = j;

                    break;
                }
            }

            return true;
        }
        else
            isDotDisplayAfterFiguresDel = true;
    }

    static const int PyrLowerSurfaceIndex = 4;
    static const int PyrUpperSurfaceIndex = 5;

    if (!(nearCrossingRayAndFigureCoord.z >= basePyrRange.zStart + 0.00001))
        rayHitSurfaceNumber = PyrLowerSurfaceIndex;
    else if (!(nearCrossingRayAndFigureCoord.z <= basePyrRange.zEnd - 0.00001))
        rayHitSurfaceNumber = PyrUpperSurfaceIndex;

    return isDotDisplayAfterFiguresDel;
}