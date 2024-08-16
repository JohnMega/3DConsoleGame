
#include <iostream>
#include <cmath>
#include <Windows.h>
#include "Parallelepiped.h"
#include "ACameraActor.h"

Parallelepiped::Parallelepiped(float length, float width, float height, COORDS centreCoord, bool IsRotateAroundZAxis, bool IsRotateAroundXAxis,
    bool IsRotateAroundYAxis, int color) noexcept
    : length(length)
    , width(width)
    , height(height)
    , centreCoord(centreCoord)
    , IsRotateAroundZAxis(IsRotateAroundZAxis)
    , IsRotateAroundXAxis(IsRotateAroundXAxis)
    , IsRotateAroundYAxis(IsRotateAroundYAxis)
    , color(color)
    , anglesRotationSum(0)
    , parallelepipedParabMode{ .isParab = false, .parabOwnerRange = {0,0,0,0,0,0} }
    , isParalHasTexture(false)
{
    SetParRange();
    SetGroundParMainDots(groundParMainDot1, groundParMainDot2, groundParMainDot3, groundParMainDot4);
    SetExtendedGroundParMainDots();

    objType = objectType::PARALLELEPIPED;
}

COORDS Parallelepiped::GetParalSize() const noexcept
{
    return { length, width, height };
}

bool Parallelepiped::isObjHasTexture() const noexcept
{
    return isParalHasTexture;
}

void Parallelepiped::SetObjTextureMode(bool isParalHasTexture) noexcept
{
    this->isParalHasTexture = isParalHasTexture;
}

void Parallelepiped::SetObjTextureParams(const std::string& textureName, float widthTextureCount, float heightTextureCount) noexcept
{
    textureParams.textureName = textureName;
    textureParams.bitmap.open(textureName);
    textureParams.bmpPixelMatrix = textureParams.bitmap.toPixelMatrix();
    textureParams.widthTextureCount = abs(widthTextureCount);
    textureParams.heightTextureCount = abs(heightTextureCount);
}

std::string Parallelepiped::GetTextureName() const noexcept
{
    return textureParams.textureName;
}

COORDS Parallelepiped::ParalDotToLocalTextureCoords(COORDS localCentreCoord, COORDS localXVec, COORDS paralDot) const
{
    float x = dotDistance(localCentreCoord, paralDot) * cos(fromDegreesToRad(GetAngleBetweenVectors(localCentreCoord, paralDot, localXVec)));
    float y = dotDistance(localCentreCoord, paralDot) * sin(fromDegreesToRad(GetAngleBetweenVectors(localCentreCoord, paralDot, localXVec)));
    
    return { x, y, 0 };
}

COORDS Parallelepiped::GetColor(COORDS paralDot) const
{
    COORDS parUpDot1 = { (IsRotateAroundXAxis) ? (length / 2) : groundParMainDot1.x, (IsRotateAroundYAxis) ? (width / 2) : groundParMainDot1.y, (IsRotateAroundZAxis || (!IsRotateAroundXAxis && !IsRotateAroundYAxis)) ? (height / 2) : groundParMainDot1.z };
    COORDS parUpDot2 = { (IsRotateAroundXAxis) ? (length / 2) : groundParMainDot2.x, (IsRotateAroundYAxis) ? (width / 2) : groundParMainDot2.y, (IsRotateAroundZAxis || (!IsRotateAroundXAxis && !IsRotateAroundYAxis)) ? (height / 2) : groundParMainDot2.z };
    COORDS parUpDot3 = { (IsRotateAroundXAxis) ? (length / 2) : groundParMainDot3.x, (IsRotateAroundYAxis) ? (width / 2) : groundParMainDot3.y, (IsRotateAroundZAxis || (!IsRotateAroundXAxis && !IsRotateAroundYAxis)) ? (height / 2) : groundParMainDot3.z };
    COORDS parUpDot4 = { (IsRotateAroundXAxis) ? (length / 2) : groundParMainDot4.x, (IsRotateAroundYAxis) ? (width / 2) : groundParMainDot4.y, (IsRotateAroundZAxis || (!IsRotateAroundXAxis && !IsRotateAroundYAxis)) ? (height / 2) : groundParMainDot4.z };

    COORDS mainParDots[ParMainGroundDotsCount] = { groundParMainDot1 + centreCoord, groundParMainDot2 + centreCoord, groundParMainDot3 + centreCoord, groundParMainDot4 + centreCoord };
    COORDS mainParDotsUp[ParMainGroundDotsCount] = { parUpDot1 + centreCoord, parUpDot2 + centreCoord, parUpDot3 + centreCoord, parUpDot4 + centreCoord };
    COORDS surfaceDot1, surfaceDot2, surfaceDot3, surfaceDot4;

    float minDist = 10000;
    COORDS finalSurfaceDot1, finalSurfaceDot2, finalSurfaceDot3, finalSurfaceDot4;
    int finalIndex;

    for (size_t index = 0; index < ParSurfacesCount; ++index)
    {
        if (index <= 3)
        {
            surfaceDot1 = mainParDots[index];
            surfaceDot2 = mainParDots[(index == 3) ? 0 : index + 1];
            surfaceDot3 = mainParDotsUp[index];
            surfaceDot4 = mainParDotsUp[(index == 3) ? 0 : index + 1];
        }

        if (index == 4)
        {
            surfaceDot1 = groundParMainDot1 + centreCoord;
            surfaceDot2 = groundParMainDot2 + centreCoord;
            surfaceDot3 = groundParMainDot3 + centreCoord;
            surfaceDot4 = groundParMainDot4 + centreCoord;
        }

        if (index == 5)
        {
            surfaceDot1 = parUpDot1 + centreCoord;
            surfaceDot2 = parUpDot2 + centreCoord;
            surfaceDot3 = parUpDot3 + centreCoord;
            surfaceDot4 = parUpDot4 + centreCoord;
        }

        if (abs(GetSurfaceValue(surfaceDot1, surfaceDot2, surfaceDot3, paralDot)) < minDist)
        {
            finalSurfaceDot1 = surfaceDot1;
            finalSurfaceDot2 = surfaceDot2;
            finalSurfaceDot3 = surfaceDot3;
            finalSurfaceDot4 = surfaceDot4;
            finalIndex = index;
            minDist = abs(GetSurfaceValue(surfaceDot1, surfaceDot2, surfaceDot3, paralDot));
        }
    }

    COORDS newParalDot = ParalDotToLocalTextureCoords(finalSurfaceDot3, finalSurfaceDot4, paralDot);
    float surfaceHeight = dotDistance(finalSurfaceDot3, (finalIndex <= 3 ? finalSurfaceDot1 : finalSurfaceDot2));
    float surfaceWidth = dotDistance(finalSurfaceDot3, finalSurfaceDot4);
    COORDS scalingNewParalDot = { newParalDot.x * (textureParams.bitmap.wight / surfaceWidth) * textureParams.widthTextureCount, newParalDot.y * (textureParams.bitmap.height / surfaceHeight) * textureParams.heightTextureCount, newParalDot.z };

    int line, column;
    try
    {
        line = abs((int)scalingNewParalDot.y) % (int)textureParams.bitmap.height;
        column = abs((int)scalingNewParalDot.x) % (int)textureParams.bitmap.wight;
        textureParams.bmpPixelMatrix.at(line).at(column);
    }
    catch (...)
    {
        line = column = 0;
    }

    BmpPixel currentPolygonRGBColor = textureParams.bmpPixelMatrix[line][column];
    return { (float)currentPolygonRGBColor.red, (float)currentPolygonRGBColor.green, (float)currentPolygonRGBColor.blue };
}

COORDS Parallelepiped::DownDotCollisionHandle(COORDS mainDot, COORDS finalSurfaceDot1, COORDS finalSurfaceDot2, COORDS finalSurfaceDot3)
{
    COORDS surfaceAndNormCross = GetCrossingSurfaceAndLineCoords(finalSurfaceDot1, finalSurfaceDot2, finalSurfaceDot3, mainDot - centreCoord, (mainDot - centreCoord) + COORDS{ 0, 0, 1 });

    return (COORDS{ 0,0,1 } * (dotDistance(surfaceAndNormCross, mainDot - centreCoord) + 0.01));
}

COORDS Parallelepiped::UpDotCollisionHandle(COORDS mainDot, COORDS finalSurfaceDot1, COORDS finalSurfaceDot2, COORDS finalSurfaceDot3)
{
    COORDS surfaceAndNormCross = GetCrossingSurfaceAndLineCoords(finalSurfaceDot1, finalSurfaceDot2, finalSurfaceDot3, mainDot - centreCoord, (mainDot - centreCoord) + COORDS{ 0, 0, -1 });

    return (COORDS{ 0,0,1 } * (dotDistance(surfaceAndNormCross, mainDot - centreCoord) + 0.01));
}

COORDS Parallelepiped::CollisionHandle(COORDS mainDotDir, COORDS mainDot)
{
    if (!IsParaboloid())
    {
        COORDS parUpDot1 = { (IsRotateAroundXAxis) ? (length / 2) + extendValue : extendedGroundParMainDot1.x, (IsRotateAroundYAxis) ? (width / 2) + extendValue : extendedGroundParMainDot1.y, (IsRotateAroundZAxis || (!IsRotateAroundXAxis && !IsRotateAroundYAxis)) ? (height / 2) + extendValue : extendedGroundParMainDot1.z };
        COORDS parUpDot2 = { (IsRotateAroundXAxis) ? (length / 2) + extendValue : extendedGroundParMainDot2.x, (IsRotateAroundYAxis) ? (width / 2) + extendValue : extendedGroundParMainDot2.y, (IsRotateAroundZAxis || (!IsRotateAroundXAxis && !IsRotateAroundYAxis)) ? (height / 2) + extendValue : extendedGroundParMainDot2.z };
        COORDS parUpDot3 = { (IsRotateAroundXAxis) ? (length / 2) + extendValue : extendedGroundParMainDot3.x, (IsRotateAroundYAxis) ? (width / 2) + extendValue : extendedGroundParMainDot3.y, (IsRotateAroundZAxis || (!IsRotateAroundXAxis && !IsRotateAroundYAxis)) ? (height / 2) + extendValue : extendedGroundParMainDot3.z };
        COORDS parUpDot4 = { (IsRotateAroundXAxis) ? (length / 2) + extendValue : extendedGroundParMainDot4.x, (IsRotateAroundYAxis) ? (width / 2) + extendValue : extendedGroundParMainDot4.y, (IsRotateAroundZAxis || (!IsRotateAroundXAxis && !IsRotateAroundYAxis)) ? (height / 2) + extendValue : extendedGroundParMainDot4.z };

        float surfaceValue1 = GetSurfaceValue(extendedGroundParMainDot1, extendedGroundParMainDot2, parUpDot1, mainDot - centreCoord);
        float reverseSurfaceValue1 = GetSurfaceValue(extendedGroundParMainDot1, extendedGroundParMainDot2, parUpDot1, extendedGroundParMainDot3);
        float surfaceValue2 = GetSurfaceValue(extendedGroundParMainDot2, extendedGroundParMainDot3, parUpDot2, mainDot - centreCoord);
        float reverseSurfaceValue2 = GetSurfaceValue(extendedGroundParMainDot2, extendedGroundParMainDot3, parUpDot2, extendedGroundParMainDot1);
        float surfaceValue3 = GetSurfaceValue(extendedGroundParMainDot3, extendedGroundParMainDot4, parUpDot3, mainDot - centreCoord);
        float reverseSurfaceValue3 = GetSurfaceValue(extendedGroundParMainDot3, extendedGroundParMainDot4, parUpDot3, extendedGroundParMainDot1);
        float surfaceValue4 = GetSurfaceValue(extendedGroundParMainDot4, extendedGroundParMainDot1, parUpDot4, mainDot - centreCoord);
        float reverseSurfaceValue4 = GetSurfaceValue(extendedGroundParMainDot4, extendedGroundParMainDot1, parUpDot4, extendedGroundParMainDot2);
        float surfaceValue5 = GetSurfaceValue(extendedGroundParMainDot1, extendedGroundParMainDot2, extendedGroundParMainDot3, mainDot - centreCoord);
        float reverseSurfaceValue5 = GetSurfaceValue(extendedGroundParMainDot1, extendedGroundParMainDot2, extendedGroundParMainDot3, parUpDot4);
        float surfaceValue6 = GetSurfaceValue(parUpDot1, parUpDot2, parUpDot3, mainDot - centreCoord);
        float reverseSurfaceValue6 = GetSurfaceValue(parUpDot1, parUpDot2, parUpDot3, extendedGroundParMainDot1);
        
        if ((IsSign(surfaceValue1, reverseSurfaceValue1) && IsSign(surfaceValue2, reverseSurfaceValue2) && IsSign(surfaceValue3, reverseSurfaceValue3) && IsSign(surfaceValue4, reverseSurfaceValue4) && IsSign(surfaceValue5, reverseSurfaceValue5)) && IsSign(surfaceValue6, reverseSurfaceValue6))
        {
            COORDS mainParDots[ParMainGroundDotsCount] = { extendedGroundParMainDot1, extendedGroundParMainDot2, extendedGroundParMainDot3, extendedGroundParMainDot4 };
            COORDS mainParDotsUp[ParMainGroundDotsCount] = { parUpDot1, parUpDot2, parUpDot3, parUpDot4 };
            COORDS surfaceDot1, surfaceDot2, surfaceDot3, surfaceDot4;
            COORDS normVec, dotProjOnSurface;

            float minDist = 10000;
            COORDS finalSurfaceDot1, finalSurfaceDot2, finalSurfaceDot3;
            COORDS finalDotProjOnSurface;
            for (size_t index = 0; index < ParSurfacesCount; ++index)
            {
                if (index <= 3)
                {
                    surfaceDot1 = mainParDots[index];
                    surfaceDot2 = mainParDots[(index == 3) ? 0 : index + 1];
                    surfaceDot3 = mainParDotsUp[index];
                    surfaceDot4 = mainParDotsUp[(index == 3) ? 0 : index + 1];
                }

                if (index == 4)
                {
                    surfaceDot1 = extendedGroundParMainDot1;
                    surfaceDot2 = extendedGroundParMainDot2;
                    surfaceDot3 = extendedGroundParMainDot3;
                    surfaceDot4 = extendedGroundParMainDot4;
                }

                if (index == 5)
                {
                    surfaceDot1 = parUpDot1;
                    surfaceDot2 = parUpDot2;
                    surfaceDot3 = parUpDot3;
                    surfaceDot4 = parUpDot4;
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

            if (mainDotDir.x == 0 && mainDotDir.y == 0 && mainDotDir.z < 0)
            {
                return DownDotCollisionHandle(mainDot, finalSurfaceDot1, finalSurfaceDot2, finalSurfaceDot3);
            }
            else if (mainDotDir.x == 0 && mainDotDir.y == 0 && mainDotDir.z > 0)
            {
                return UpDotCollisionHandle(mainDot, finalSurfaceDot1, finalSurfaceDot2, finalSurfaceDot3);
            }

            normVec = vectorNormalize(GetSurfaceNormVec(finalSurfaceDot1, finalSurfaceDot2, finalSurfaceDot3), { 0,0,0 });
            if (dotDistance((normVec / 100) + finalDotProjOnSurface, (mainDot - centreCoord)) < dotDistance(((normVec / 100) * (-1)) + finalDotProjOnSurface, (mainDot - centreCoord)))
                normVec = normVec * (-1);

            COORDS surfaceAndNormCross = GetCrossingSurfaceAndLineCoords(finalSurfaceDot1, finalSurfaceDot2, finalSurfaceDot3, mainDot - centreCoord, (mainDot - centreCoord) + normVec);
            return (normVec * (dotDistance(surfaceAndNormCross, mainDot - centreCoord) + 0.01));
        }
    }
    else
    {
        if (IsDotInRange(mainDot))
        {
            float f = (parRange.zEnd);
            float h = -centreCoord.x, j = -centreCoord.y;
            float a = pow(parRange.xStart + h, 2) / (f - parRange.zStart);
            float b = pow(parRange.yStart + j, 2) / (f - parRange.zStart);

            COORDS trueParabDot = mainDot;
            trueParabDot.z = -pow(mainDot.x + h, 2) / a - pow(mainDot.y + j, 2) / b + f;

            if (mainDot.z <= trueParabDot.z)
                return (COORDS{ 0,0,1 } * (dotDistance(trueParabDot, mainDot) + 0.01));
        }
    }

    return { 0,0,0 };
}

COORDS Parallelepiped::GetCentreCoord() const noexcept
{
    return centreCoord;
}

COORDS& Parallelepiped::SetCentreCoord() noexcept
{
    return centreCoord;
}

Range& Parallelepiped::SetRange() noexcept
{
    return parRange;
}

Range Parallelepiped::GetRange() const noexcept
{
    return parRange;
}

void Parallelepiped::UpdateRange(size_t flag) noexcept
{
    if (flag == 2)
    {
        parRange.xStart = centreCoord.x - (length / 2);
        parRange.xEnd = centreCoord.x + (length / 2);
        parRange.yStart = centreCoord.y - (width / 2);
        parRange.yEnd = centreCoord.y + (width / 2);
    }
    else
        SetParRange();

    if (flag == 1)
    {
        SetGroundParMainDots(groundParMainDot1, groundParMainDot2, groundParMainDot3, groundParMainDot4);
        SetExtendedGroundParMainDots();
    }
}

objectType Parallelepiped::GetObjType() const noexcept
{
    return objType;
}

bool Parallelepiped::IsRotate() const noexcept
{
    return ((IsRotateAroundZAxis || IsRotateAroundXAxis || IsRotateAroundYAxis) ? true : false);
}

int Parallelepiped::GetColor() const noexcept
{
    return color;
}

int& Parallelepiped::SetColor() noexcept
{
    return color;
}

void Parallelepiped::SurfacesValuesCalc(COORDS checkedDot, COORDS unnecessaryShapeMainDot1, COORDS unnecessaryShapeMainDot2, COORDS unnecessaryShapeMainDot3, COORDS unnecessaryShapeMainDot4, COORDS unnecessaryShapeMainDot5, COORDS unnecessaryShapeMainDot6,
    float& surfaceValue1, float& reverseSurfaceValue1, float& surfaceValue2, float& reverseSurfaceValue2, float& surfaceValue3, float& reverseSurfaceValue3, float& surfaceValue4, float& reverseSurfaceValue4, float& surfaceValue5, float& reverseSurfaceValue5) const noexcept
{
    surfaceValue1 = GetSurfaceValue(unnecessaryShapeMainDot1, unnecessaryShapeMainDot5, unnecessaryShapeMainDot2, checkedDot - centreCoord);
    reverseSurfaceValue1 = GetSurfaceValue(unnecessaryShapeMainDot1, unnecessaryShapeMainDot5, unnecessaryShapeMainDot2, unnecessaryShapeMainDot6);
    surfaceValue2 = GetSurfaceValue(unnecessaryShapeMainDot3, unnecessaryShapeMainDot6, unnecessaryShapeMainDot4, checkedDot - centreCoord);
    reverseSurfaceValue2 = GetSurfaceValue(unnecessaryShapeMainDot3, unnecessaryShapeMainDot6, unnecessaryShapeMainDot4, unnecessaryShapeMainDot5);
    surfaceValue3 = GetSurfaceValue(unnecessaryShapeMainDot5, unnecessaryShapeMainDot6, unnecessaryShapeMainDot2, checkedDot - centreCoord);
    reverseSurfaceValue3 = GetSurfaceValue(unnecessaryShapeMainDot5, unnecessaryShapeMainDot6, unnecessaryShapeMainDot2, unnecessaryShapeMainDot1);
    surfaceValue4 = GetSurfaceValue(unnecessaryShapeMainDot5, unnecessaryShapeMainDot6, unnecessaryShapeMainDot1, checkedDot - centreCoord);
    reverseSurfaceValue4 = GetSurfaceValue(unnecessaryShapeMainDot5, unnecessaryShapeMainDot6, unnecessaryShapeMainDot1, unnecessaryShapeMainDot2);
    surfaceValue5 = GetSurfaceValue(unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, checkedDot - centreCoord);
    reverseSurfaceValue5 = GetSurfaceValue(unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, unnecessaryShapeMainDot5);
}

void Parallelepiped::SetParRange() noexcept
{
    parRange.xStart = centreCoord.x - (length / 2);
    parRange.xEnd = centreCoord.x + (length / 2);
    parRange.yStart = centreCoord.y - (width / 2);
    parRange.yEnd = centreCoord.y + (width / 2);
    parRange.zStart = centreCoord.z - (height / 2);
    parRange.zEnd = centreCoord.z + (height / 2);
}

void Parallelepiped::SetGroundParMainDots(COORDS& groundMainDot1, COORDS& groundMainDot2, COORDS& groundMainDot3, COORDS& groundMainDot4) noexcept
{
    if (IsRotate())
    {
        groundMainDot1 = { parRange.xStart - centreCoord.x, parRange.yStart - centreCoord.y, parRange.zStart - centreCoord.z };
        groundMainDot2 = { parRange.xStart - centreCoord.x, (IsRotateAroundXAxis || IsRotateAroundYAxis) ? parRange.yStart - centreCoord.y : parRange.yEnd - centreCoord.y, (IsRotateAroundZAxis) ? parRange.zStart - centreCoord.z : parRange.zEnd - centreCoord.z };
        groundMainDot3 = { (IsRotateAroundXAxis) ? parRange.xStart - centreCoord.x : parRange.xEnd - centreCoord.x, (IsRotateAroundYAxis) ? parRange.yStart - centreCoord.y : parRange.yEnd - centreCoord.y, (IsRotateAroundZAxis) ? parRange.zStart - centreCoord.z : parRange.zEnd - centreCoord.z };
        groundMainDot4 = { (IsRotateAroundXAxis) ? parRange.xStart - centreCoord.x : parRange.xEnd - centreCoord.x, (IsRotateAroundZAxis || IsRotateAroundYAxis) ? parRange.yStart - centreCoord.y : parRange.yEnd - centreCoord.y, parRange.zStart - centreCoord.z };
    }
    else
    {
        groundMainDot1 = { parRange.xStart - centreCoord.x, parRange.yStart - centreCoord.y, parRange.zStart - centreCoord.z };
        groundMainDot2 = { parRange.xStart - centreCoord.x, parRange.yEnd - centreCoord.y, parRange.zStart - centreCoord.z };
        groundMainDot3 = { parRange.xEnd - centreCoord.x, parRange.yEnd - centreCoord.y, parRange.zStart - centreCoord.z };
        groundMainDot4 = { parRange.xEnd - centreCoord.x, parRange.yStart - centreCoord.y, parRange.zStart - centreCoord.z };
    }
}

void Parallelepiped::SetExtendedGroundParMainDots() noexcept
{
    if (IsRotate())
    {
        extendedGroundParMainDot1 = { groundParMainDot1.x - extendValue, groundParMainDot1.y - extendValue, groundParMainDot1.z - extendValue };
        extendedGroundParMainDot2 = { groundParMainDot2.x - extendValue, (IsRotateAroundXAxis || IsRotateAroundYAxis) ? groundParMainDot2.y - extendValue : groundParMainDot2.y + extendValue, (IsRotateAroundZAxis) ? groundParMainDot2.z - extendValue : groundParMainDot2.z + extendValue };
        extendedGroundParMainDot3 = { (IsRotateAroundXAxis) ? groundParMainDot3.x - extendValue : groundParMainDot3.x + extendValue, (IsRotateAroundYAxis) ? groundParMainDot3.y - extendValue : groundParMainDot3.y + extendValue, (IsRotateAroundZAxis) ? groundParMainDot3.z - extendValue : groundParMainDot3.z + extendValue };
        extendedGroundParMainDot4 = { (IsRotateAroundXAxis) ? groundParMainDot4.x - extendValue : groundParMainDot4.x + extendValue, (IsRotateAroundZAxis || IsRotateAroundYAxis) ? groundParMainDot4.y - extendValue : groundParMainDot4.y + extendValue, groundParMainDot4.z - extendValue };
    }
    else
    {
        extendedGroundParMainDot1 = { groundParMainDot1.x - extendValue, groundParMainDot1.y - extendValue, groundParMainDot1.z - extendValue };
        extendedGroundParMainDot2 = { groundParMainDot2.x - extendValue, groundParMainDot2.y + extendValue, groundParMainDot2.z - extendValue };
        extendedGroundParMainDot3 = { groundParMainDot3.x + extendValue, groundParMainDot3.y + extendValue, groundParMainDot3.z - extendValue };
        extendedGroundParMainDot4 = { groundParMainDot4.x + extendValue, groundParMainDot4.y - extendValue, groundParMainDot4.z - extendValue };
    }
}

bool Parallelepiped::IsRayCrossNotRotatedFigure(COORDS rayStartCoord, COORDS surfaceDot) const
{
    static const float sufficientNarrowingValue = 0.0001F;

    COORDS normVec;

    normVec = { (!(surfaceDot.x >= parRange.xStart + sufficientNarrowingValue)) ? surfaceDot.x - 1 : ((!(surfaceDot.x <= parRange.xEnd - sufficientNarrowingValue)) ? surfaceDot.x + 1 : surfaceDot.x),
        (!(surfaceDot.y >= parRange.yStart + sufficientNarrowingValue)) ? surfaceDot.y - 1 : ((!(surfaceDot.y <= parRange.yEnd - sufficientNarrowingValue)) ? surfaceDot.y + 1 : surfaceDot.y),
        (!(surfaceDot.z >= parRange.zStart + sufficientNarrowingValue)) ? surfaceDot.z - 1 : ((!(surfaceDot.z <= parRange.zEnd - sufficientNarrowingValue)) ? surfaceDot.z + 1 : surfaceDot.z) };

    if (GetAngleBetweenVectors(surfaceDot, rayStartCoord, normVec) > 90)
    {
        return true;
    }
    else
        return false;
}

bool Parallelepiped::IsRayCrossParaboloid(COORDS rayStartCoord, COORDS surfaceDot) const
{
    float f = (parRange.zEnd);
    float h = -centreCoord.x, j = -centreCoord.y;
    float a = pow(parRange.xStart + h, 2) / (f - parRange.zStart);
    float b = pow(parRange.yStart + j, 2) / (f - parRange.zStart);

    float fz1 = f - rayStartCoord.z;
    float hx1 = h + rayStartCoord.x;
    float jy1 = j + rayStartCoord.y;
    float Z = surfaceDot.z - rayStartCoord.z;
    float X = surfaceDot.x - rayStartCoord.x;
    float Y = surfaceDot.y - rayStartCoord.y;

    float g1 = (a * b * Z * Z + (4 * a * jy1 * Y + 4 * b * hx1 * X) * Z + (4 * a * fz1 - 4 * hx1 * hx1) * Y * Y + 8 * hx1 * jy1 * X * Y + (4 * b * fz1 - 4 * jy1 * jy1) * X * X) / (a * b);
    float g2 = (2 * a * Y * Y + 2 * b * X * X);

    if (g1 < 0 || (g2 >= -0.01 && g2 <= 0.01)) return false;

    float res1 = (a * b * sqrt(g1)) / (g2)-(a * b * Z) / (g2)-(2 * a * jy1 * Y) / (g2)-(2 * b * hx1 * X) / (g2);
    float res2 = -(a * b * sqrt(g1)) / (g2)-(a * b * Z) / (g2)-(2 * a * jy1 * Y) / (g2)-(2 * b * hx1 * X) / (g2);

    COORDS res1Dot = { X * res1 + rayStartCoord.x, Y * res1 + rayStartCoord.y, Z * res1 + rayStartCoord.z };
    COORDS res2Dot = { X * res2 + rayStartCoord.x, Y * res2 + rayStartCoord.y, Z * res2 + rayStartCoord.z };

    float zz1 = -pow(res1Dot.x + h, 2) / a - pow(res1Dot.y + j, 2) / b + f;
    float zz2 = -pow(res2Dot.x + h, 2) / a - pow(res2Dot.y + j, 2) / b + f;

    if (IsDotInRange(res1Dot) && IsDotInRange(res2Dot))
    {
        if ((zz1 >= res1Dot.z - 0.01 && zz1 <= res1Dot.z + 0.01) && (zz2 >= res2Dot.z - 0.01 && zz2 <= res2Dot.z + 0.01))
        {
            if (res1Dot.x >= min(rayStartCoord.x, surfaceDot.x) && res1Dot.x <= max(rayStartCoord.x, surfaceDot.x) && res1Dot.y >= min(rayStartCoord.y, surfaceDot.y) &&
                res1Dot.y <= max(rayStartCoord.y, surfaceDot.y) && res1Dot.z >= min(rayStartCoord.z, surfaceDot.z) && res1Dot.z <= max(rayStartCoord.z, surfaceDot.z))
                return true;

            if (res2Dot.x >= min(rayStartCoord.x, surfaceDot.x) && res2Dot.x <= max(rayStartCoord.x, surfaceDot.x) && res2Dot.y >= min(rayStartCoord.y, surfaceDot.y) &&
                res2Dot.y <= max(rayStartCoord.y, surfaceDot.y) && res2Dot.z >= min(rayStartCoord.z, surfaceDot.z) && res2Dot.z <= max(rayStartCoord.z, surfaceDot.z))
                return true;
        }
    }

    if (IsDotInRange(res1Dot))
    {
        if ((zz1 >= res1Dot.z - 0.01 && zz1 <= res1Dot.z + 0.01))
        {
            if (res1Dot.x >= min(rayStartCoord.x, surfaceDot.x) && res1Dot.x <= max(rayStartCoord.x, surfaceDot.x) && res1Dot.y >= min(rayStartCoord.y, surfaceDot.y) &&
                res1Dot.y <= max(rayStartCoord.y, surfaceDot.y) && res1Dot.z >= min(rayStartCoord.z, surfaceDot.z) && res1Dot.z <= max(rayStartCoord.z, surfaceDot.z))
                return true;
        }
    }

    if (IsDotInRange(res2Dot))
    {
        if ((zz2 >= res2Dot.z - 0.01 && zz2 <= res2Dot.z + 0.01))
        {
            if (res2Dot.x >= min(rayStartCoord.x, surfaceDot.x) && res2Dot.x <= max(rayStartCoord.x, surfaceDot.x) && res2Dot.y >= min(rayStartCoord.y, surfaceDot.y) &&
                res2Dot.y <= max(rayStartCoord.y, surfaceDot.y) && res2Dot.z >= min(rayStartCoord.z, surfaceDot.z) && res2Dot.z <= max(rayStartCoord.z, surfaceDot.z))
                return true;
        }
    }

    return false;
}

bool Parallelepiped::IsRotatedFigureShine(COORDS lightDot, COORDS parSurfaceDot, int rayHitSurfaceNumber)
{
    static const int ParFirstNonMovingSurfaceIndex = 4;
    static const int ParSecondNonMovingSurfaceIndex = 5;

    COORDS mainParDots[ParMainGroundDotsCount] = { extendedGroundParMainDot1, extendedGroundParMainDot2, extendedGroundParMainDot3, extendedGroundParMainDot4 };
    COORDS surfaceDot1, surfaceDot2, surfaceDot3;
    size_t failedAttemptsCount = 0;

    //the case when the dot in question did move when removing unnecessary shapes
    for (size_t index = 0; index < ParSurfacesCount; ++index)
    {
        if (index < ParMovingSurfacesCount)
        {
            surfaceDot1 = mainParDots[index];
            surfaceDot2 = mainParDots[(index == 3) ? 0 : index + 1];
            surfaceDot3 = { (IsRotateAroundXAxis) ? (length / 2) : surfaceDot1.x, (IsRotateAroundYAxis) ? (width / 2) : surfaceDot1.y, (IsRotateAroundZAxis) ? (height / 2) : surfaceDot1.z };
        }

        if (index == ParFirstNonMovingSurfaceIndex)
        {
            surfaceDot1 = { groundParMainDot1.x, groundParMainDot1.y, groundParMainDot1.z };
            surfaceDot2 = { groundParMainDot2.x, groundParMainDot2.y, groundParMainDot2.z };
            surfaceDot3 = { groundParMainDot3.x, groundParMainDot3.y, groundParMainDot3.z };
        }

        if (index == ParSecondNonMovingSurfaceIndex)
        {
            surfaceDot1 = { (IsRotateAroundXAxis) ? -groundParMainDot1.x : groundParMainDot1.x, (IsRotateAroundYAxis) ? -groundParMainDot1.y : groundParMainDot1.y, (IsRotateAroundZAxis) ? -groundParMainDot1.z : groundParMainDot1.z };
            surfaceDot2 = { (IsRotateAroundXAxis) ? -groundParMainDot2.x : groundParMainDot2.x, (IsRotateAroundYAxis) ? -groundParMainDot2.y : groundParMainDot2.y, (IsRotateAroundZAxis) ? -groundParMainDot2.z : groundParMainDot2.z };
            surfaceDot3 = { (IsRotateAroundXAxis) ? -groundParMainDot3.x : groundParMainDot3.x, (IsRotateAroundYAxis) ? -groundParMainDot3.y : groundParMainDot3.y, (IsRotateAroundZAxis) ? -groundParMainDot3.z : groundParMainDot3.z };
        }

        if (index == rayHitSurfaceNumber)
        {
            if (IsRayHitFigureSurfaceDot(surfaceDot1, surfaceDot2, surfaceDot3, lightDot, parSurfaceDot, centreCoord))
                return true;
        }
        else
            ++failedAttemptsCount;
    }

    COORDS parCoord1, parCoord2, parCoord3, parCoord4;

    //the case when the dot in question did not move when removing unnecessary shapes
    if (failedAttemptsCount == ParSurfacesCount)
    {
        SetGroundParMainDots(parCoord1, parCoord2, parCoord3, parCoord4);
        mainParDots[0] = parCoord1; mainParDots[1] = parCoord2; mainParDots[2] = parCoord3; mainParDots[3] = parCoord4;

        static const int ParNonBaseSurfacesCount = 4;
        for (size_t index = 0; index < ParNonBaseSurfacesCount; ++index)
        {
            surfaceDot1 = mainParDots[index];
            surfaceDot2 = mainParDots[(index == 3) ? 0 : index + 1];
            surfaceDot3 = { (IsRotateAroundXAxis) ? (length / 2) : surfaceDot1.x, (IsRotateAroundYAxis) ? (width / 2) : surfaceDot1.y, (IsRotateAroundZAxis) ? (height / 2) : surfaceDot1.z };

            if (abs(GetSurfaceValue(surfaceDot1, surfaceDot2, surfaceDot3, parSurfaceDot - centreCoord)) < extendValue)
            {
                if (IsRayHitFigureSurfaceDot(surfaceDot1, surfaceDot2, surfaceDot3, lightDot, parSurfaceDot, centreCoord))
                    return true;
            }
        }
    }

    return false;
}

bool Parallelepiped::IsParaboloid() const noexcept
{
    return parallelepipedParabMode.isParab;
}

bool Parallelepiped::IsDotInRange(COORDS dot) const noexcept
{
    if (dot.x >= parRange.xStart && dot.x <= parRange.xEnd && dot.y >= parRange.yStart && dot.y <= parRange.yEnd
        && dot.z >= parRange.zStart && dot.z <= parRange.zEnd)
        return true;

    return false;
}

bool Parallelepiped::ToParab(COORDS cameraNormalizeRay, COORDS cameraCoord, COORDS& nearCrossingRayAndFigureCoord, float farCrossingRayAndFigureValue) noexcept
{
    float f = (parRange.zEnd);
    float h = -centreCoord.x, j = -centreCoord.y;
    float a = pow(parRange.xStart + h, 2) / (f - parRange.zStart);
    float b = pow(parRange.yStart + j, 2) / (f - parRange.zStart);

    float fz1 = f - cameraCoord.z;
    float hx1 = h + cameraCoord.x;
    float jy1 = j + cameraCoord.y;
    float Z = nearCrossingRayAndFigureCoord.z - cameraCoord.z;
    float X = nearCrossingRayAndFigureCoord.x - cameraCoord.x;
    float Y = nearCrossingRayAndFigureCoord.y - cameraCoord.y;

    float g1 = (a * b * Z * Z + (4 * a * jy1 * Y + 4 * b * hx1 * X) * Z + (4 * a * fz1 - 4 * hx1 * hx1) * Y * Y + 8 * hx1 * jy1 * X * Y + (4 * b * fz1 - 4 * jy1 * jy1) * X * X) / (a * b);
    float g2 = (2 * a * Y * Y + 2 * b * X * X);

    if (g1 < 0 || (g2 >= -0.01 && g2 <= 0.01)) return false;

    float res1 = (a * b * sqrt(g1)) / (g2)-(a * b * Z) / (g2)-(2 * a * jy1 * Y) / (g2)-(2 * b * hx1 * X) / (g2);
    float res2 = -(a * b * sqrt(g1)) / (g2)-(a * b * Z) / (g2)-(2 * a * jy1 * Y) / (g2)-(2 * b * hx1 * X) / (g2);

    COORDS res1Dot = { X * res1 + cameraCoord.x, Y * res1 + cameraCoord.y, Z * res1 + cameraCoord.z };
    COORDS res2Dot = { X * res2 + cameraCoord.x, Y * res2 + cameraCoord.y, Z * res2 + cameraCoord.z };

    float zz1 = -pow(res1Dot.x + h, 2) / a - pow(res1Dot.y + j, 2) / b + f;
    float zz2 = -pow(res2Dot.x + h, 2) / a - pow(res2Dot.y + j, 2) / b + f;

    if (IsDotInRange(res1Dot) && IsDotInRange(res2Dot))
    {
        if ((zz1 >= res1Dot.z - 0.01 && zz1 <= res1Dot.z + 0.01) && (zz2 >= res2Dot.z - 0.01 && zz2 <= res2Dot.z + 0.01))
        {
            nearCrossingRayAndFigureCoord = (dotDistance(res1Dot, cameraCoord) < dotDistance(res2Dot, cameraCoord) ? res1Dot : res2Dot);

            if (nearCrossingRayAndFigureCoord.x >= parallelepipedParabMode.parabOwnerRange.xStart && nearCrossingRayAndFigureCoord.x <= parallelepipedParabMode.parabOwnerRange.xEnd
                && nearCrossingRayAndFigureCoord.y >= parallelepipedParabMode.parabOwnerRange.yStart && nearCrossingRayAndFigureCoord.y <= parallelepipedParabMode.parabOwnerRange.yEnd)
                return true;
            else
                return false;
        }
    }

    if (IsDotInRange(res1Dot))
    {
        if ((zz1 >= res1Dot.z - 0.01 && zz1 <= res1Dot.z + 0.01))
        {
            nearCrossingRayAndFigureCoord = res1Dot;

            if (nearCrossingRayAndFigureCoord.x >= parallelepipedParabMode.parabOwnerRange.xStart && nearCrossingRayAndFigureCoord.x <= parallelepipedParabMode.parabOwnerRange.xEnd
                && nearCrossingRayAndFigureCoord.y >= parallelepipedParabMode.parabOwnerRange.yStart && nearCrossingRayAndFigureCoord.y <= parallelepipedParabMode.parabOwnerRange.yEnd)
                return true;
            else
                return false;
        }
    }

    if (IsDotInRange(res2Dot))
    {
        if ((zz2 >= res2Dot.z - 0.01 && zz2 <= res2Dot.z + 0.01))
        {
            nearCrossingRayAndFigureCoord = res2Dot;

            if (nearCrossingRayAndFigureCoord.x >= parallelepipedParabMode.parabOwnerRange.xStart && nearCrossingRayAndFigureCoord.x <= parallelepipedParabMode.parabOwnerRange.xEnd
                && nearCrossingRayAndFigureCoord.y >= parallelepipedParabMode.parabOwnerRange.yStart && nearCrossingRayAndFigureCoord.y <= parallelepipedParabMode.parabOwnerRange.yEnd)
                return true;
            else
                return false;
        }
    }

    return false;
}

bool Parallelepiped::IsRayCrossRotatedFigure(COORDS startRayCoord, COORDS endRayCoord) const noexcept
{
    static const int ParFirstNonMovingSurfaceIndex = 4;
    static const int ParSecondNonMovingSurfaceIndex = 5;

    COORDS mainParDots[ParMainGroundDotsCount] = { groundParMainDot1, groundParMainDot2, groundParMainDot3, groundParMainDot4 };
    COORDS surfaceDot1, surfaceDot2, surfaceDot3;

    for (size_t index = 0; index < ParSurfacesCount; ++index)
    {
        surfaceDot1 = mainParDots[index];
        surfaceDot2 = mainParDots[(index == 3) ? 0 : index + 1];
        surfaceDot3 = { (IsRotateAroundXAxis) ? (length / 2) : surfaceDot1.x, (IsRotateAroundYAxis) ? (width / 2) : surfaceDot1.y, (IsRotateAroundZAxis) ? (height / 2) : surfaceDot1.z };

        if (index == ParFirstNonMovingSurfaceIndex)
        {
            surfaceDot1 = { groundParMainDot1.x, groundParMainDot1.y, groundParMainDot1.z };
            surfaceDot2 = { groundParMainDot2.x, groundParMainDot2.y, groundParMainDot2.z };
            surfaceDot3 = { groundParMainDot3.x, groundParMainDot3.y, groundParMainDot3.z };
        }

        if (index == ParSecondNonMovingSurfaceIndex)
        {
            surfaceDot1 = { (IsRotateAroundXAxis) ? -groundParMainDot1.x : groundParMainDot1.x, (IsRotateAroundYAxis) ? -groundParMainDot1.y : groundParMainDot1.y, (IsRotateAroundZAxis) ? -groundParMainDot1.z : groundParMainDot1.z };
            surfaceDot2 = { (IsRotateAroundXAxis) ? -groundParMainDot2.x : groundParMainDot2.x, (IsRotateAroundYAxis) ? -groundParMainDot2.y : groundParMainDot2.y, (IsRotateAroundZAxis) ? -groundParMainDot2.z : groundParMainDot2.z };
            surfaceDot3 = { (IsRotateAroundXAxis) ? -groundParMainDot3.x : groundParMainDot3.x, (IsRotateAroundYAxis) ? -groundParMainDot3.y : groundParMainDot3.y, (IsRotateAroundZAxis) ? -groundParMainDot3.z : groundParMainDot3.z };
        }

        COORDS crossingSurfaceAndLineCoords = GetCrossingSurfaceAndLineCoords(surfaceDot1, surfaceDot2, surfaceDot3, endRayCoord - centreCoord, startRayCoord - centreCoord);

        if (crossingSurfaceAndLineCoords.x + centreCoord.x >= min(startRayCoord.x, endRayCoord.x) && crossingSurfaceAndLineCoords.x + centreCoord.x <= max(startRayCoord.x, endRayCoord.x) && crossingSurfaceAndLineCoords.y + centreCoord.y >= min(startRayCoord.y, endRayCoord.y) &&
            crossingSurfaceAndLineCoords.y + centreCoord.y <= max(startRayCoord.y, endRayCoord.y) && crossingSurfaceAndLineCoords.z + centreCoord.z >= min(startRayCoord.z, endRayCoord.z) && crossingSurfaceAndLineCoords.z + centreCoord.z <= max(startRayCoord.z, endRayCoord.z))
            if (IsDotInRotatedFigure(crossingSurfaceAndLineCoords))
                return true;
    }

    return false;
}

bool Parallelepiped::IsDotInRotatedFigure(COORDS checkedDotCoord) const noexcept
{
    COORDS extendedPeakParMainDot1, extendedPeakParMainDot2, extendedPeakParMainDot3, extendedPeakParMainDot4;
    float surfaceValue1, reverseSurfaceValue1, surfaceValue2, reverseSurfaceValue2, surfaceValue3, reverseSurfaceValue3, surfaceValue4,
        reverseSurfaceValue4, surfaceValue5, reverseSurfaceValue5, surfaceValue6, reverseSurfaceValue6;

    extendedPeakParMainDot1 = { (IsRotateAroundXAxis) ? (length / 2) + extendValue : extendedGroundParMainDot1.x, (IsRotateAroundYAxis) ? (width / 2) + extendValue : extendedGroundParMainDot1.y, (IsRotateAroundZAxis) ? (height / 2) + extendValue : extendedGroundParMainDot1.z };
    extendedPeakParMainDot2 = { (IsRotateAroundXAxis) ? (length / 2) + extendValue : extendedGroundParMainDot2.x, (IsRotateAroundYAxis) ? (width / 2) + extendValue : extendedGroundParMainDot2.y, (IsRotateAroundZAxis) ? (height / 2) + extendValue : extendedGroundParMainDot2.z };
    extendedPeakParMainDot3 = { (IsRotateAroundXAxis) ? (length / 2) + extendValue : extendedGroundParMainDot3.x, (IsRotateAroundYAxis) ? (width / 2) + extendValue : extendedGroundParMainDot3.y, (IsRotateAroundZAxis) ? (height / 2) + extendValue : extendedGroundParMainDot3.z };
    extendedPeakParMainDot4 = { (IsRotateAroundXAxis) ? (length / 2) + extendValue : extendedGroundParMainDot4.x, (IsRotateAroundYAxis) ? (width / 2) + extendValue : extendedGroundParMainDot4.y, (IsRotateAroundZAxis) ? (height / 2) + extendValue : extendedGroundParMainDot4.z };

    surfaceValue1 = GetSurfaceValue(extendedGroundParMainDot1, extendedGroundParMainDot2, extendedPeakParMainDot1, checkedDotCoord);
    reverseSurfaceValue1 = GetSurfaceValue(extendedGroundParMainDot1, extendedGroundParMainDot2, extendedPeakParMainDot1, extendedPeakParMainDot3);
    surfaceValue2 = GetSurfaceValue(extendedGroundParMainDot2, extendedGroundParMainDot3, extendedPeakParMainDot2, checkedDotCoord);
    reverseSurfaceValue2 = GetSurfaceValue(extendedGroundParMainDot2, extendedGroundParMainDot3, extendedPeakParMainDot2, extendedPeakParMainDot1);
    surfaceValue3 = GetSurfaceValue(extendedGroundParMainDot3, extendedGroundParMainDot4, extendedPeakParMainDot3, checkedDotCoord);
    reverseSurfaceValue3 = GetSurfaceValue(extendedGroundParMainDot3, extendedGroundParMainDot4, extendedPeakParMainDot3, extendedPeakParMainDot2);
    surfaceValue4 = GetSurfaceValue(extendedGroundParMainDot4, extendedGroundParMainDot1, extendedPeakParMainDot4, checkedDotCoord);
    reverseSurfaceValue4 = GetSurfaceValue(extendedGroundParMainDot4, extendedGroundParMainDot1, extendedPeakParMainDot4, extendedPeakParMainDot2);
    surfaceValue5 = GetSurfaceValue(extendedGroundParMainDot1, extendedGroundParMainDot2, extendedGroundParMainDot3, checkedDotCoord);
    reverseSurfaceValue5 = GetSurfaceValue(extendedGroundParMainDot1, extendedGroundParMainDot2, extendedGroundParMainDot3, extendedPeakParMainDot1);
    surfaceValue6 = GetSurfaceValue(extendedPeakParMainDot1, extendedPeakParMainDot2, extendedPeakParMainDot3, checkedDotCoord);
    reverseSurfaceValue6 = GetSurfaceValue(extendedPeakParMainDot1, extendedPeakParMainDot2, extendedPeakParMainDot3, extendedGroundParMainDot1);

    if (IsSign(surfaceValue1, reverseSurfaceValue1) && IsSign(surfaceValue2, reverseSurfaceValue2)
        && IsSign(surfaceValue3, reverseSurfaceValue3) && IsSign(surfaceValue4, reverseSurfaceValue4)
        && IsSign(surfaceValue5, reverseSurfaceValue5) && IsSign(surfaceValue6, reverseSurfaceValue6))
        return true;

    return false;
}

void Parallelepiped::Rotation(float angle, size_t flag) noexcept
{
    if (flag == -1 && IsRotate() == false) return;
    else if (flag == -2) angle = anglesRotationSum;

    if (IsRotate() == false)
    {
        if (flag == 0) IsRotateAroundXAxis = true;
        else if (flag == 1) IsRotateAroundYAxis = true;
        else if (flag == 2) IsRotateAroundZAxis = true;

        UpdateRange(1);
    }

    COORDS mainGroundDots[ParMainGroundDotsCount] = { groundParMainDot1, groundParMainDot2, groundParMainDot3, groundParMainDot4 };
    COORDS mainExtendedGroundDots[ParMainGroundDotsCount] = { extendedGroundParMainDot1, extendedGroundParMainDot2, extendedGroundParMainDot3, extendedGroundParMainDot4 };

    //Ground main and extended dots rotation
    for (size_t i = 0; i < ParMainGroundDotsCount * 2; ++i)
    {
        GroundDotRotation((i < ParMainGroundDotsCount) ? mainGroundDots[i] : mainExtendedGroundDots[i - ParMainGroundDotsCount], angle);
    }

    groundParMainDot1 = mainGroundDots[0]; groundParMainDot2 = mainGroundDots[1]; groundParMainDot3 = mainGroundDots[2]; groundParMainDot4 = mainGroundDots[3];
    extendedGroundParMainDot1 = mainExtendedGroundDots[0]; extendedGroundParMainDot2 = mainExtendedGroundDots[1]; extendedGroundParMainDot3 = mainExtendedGroundDots[2]; extendedGroundParMainDot4 = mainExtendedGroundDots[3];

    //maxLengths And Widths Calc
    maxNonRotatedParLenght = maxNonRotatedParWidth = maxNonRotatedExtendedParLenght = maxNonRotatedExtendedParWidth = -10000;
    for (size_t i = 0; i < ParMainGroundDotsCount; ++i)
    {
        maxNonRotatedParLenght = ((maxNonRotatedParLenght < abs((IsRotateAroundXAxis) ? mainGroundDots[i].y : mainGroundDots[i].x)) ? abs((IsRotateAroundXAxis) ? mainGroundDots[i].y : mainGroundDots[i].x) : maxNonRotatedParLenght);
        maxNonRotatedParWidth = ((maxNonRotatedParWidth < abs((IsRotateAroundXAxis || IsRotateAroundYAxis) ? mainGroundDots[i].z : mainGroundDots[i].y)) ? abs((IsRotateAroundXAxis || IsRotateAroundYAxis) ? mainGroundDots[i].z : mainGroundDots[i].y) : maxNonRotatedParWidth);
        maxNonRotatedExtendedParLenght = ((maxNonRotatedExtendedParLenght < abs((IsRotateAroundXAxis) ? mainExtendedGroundDots[i].y : mainExtendedGroundDots[i].x)) ? abs((IsRotateAroundXAxis) ? mainExtendedGroundDots[i].y : mainExtendedGroundDots[i].x) : maxNonRotatedExtendedParLenght);
        maxNonRotatedExtendedParWidth = ((maxNonRotatedExtendedParWidth < abs((IsRotateAroundXAxis || IsRotateAroundYAxis) ? mainExtendedGroundDots[i].z : mainExtendedGroundDots[i].y)) ?
            abs((IsRotateAroundXAxis || IsRotateAroundYAxis) ? mainExtendedGroundDots[i].z : mainExtendedGroundDots[i].y) : maxNonRotatedExtendedParWidth);
    }

    //include new ParRange Values
    float maxNonRotatedParWidthOrLength = (IsRotateAroundZAxis ? maxNonRotatedParWidth : maxNonRotatedParLenght);

    parRange.xStart = ((IsRotateAroundXAxis) ? parRange.xStart : -maxNonRotatedParLenght + centreCoord.x); parRange.xEnd = ((IsRotateAroundXAxis) ? parRange.xEnd : maxNonRotatedParLenght + centreCoord.x);
    parRange.yStart = ((IsRotateAroundXAxis || IsRotateAroundZAxis) ? -maxNonRotatedParWidthOrLength + centreCoord.y : parRange.yStart); parRange.yEnd = ((IsRotateAroundXAxis || IsRotateAroundZAxis) ? maxNonRotatedParWidthOrLength + centreCoord.y : parRange.yEnd);
    parRange.zStart = ((IsRotateAroundXAxis || IsRotateAroundYAxis) ? -maxNonRotatedParWidth + centreCoord.z : parRange.zStart); parRange.zEnd = ((IsRotateAroundXAxis || IsRotateAroundYAxis) ? maxNonRotatedParWidth + centreCoord.z : parRange.zEnd);

    anglesRotationSum += (angle == anglesRotationSum ? 0 : angle);
}

void Parallelepiped::GroundDotRotation(COORDS& mainDot, float angle) const noexcept
{
    mainDot = { (IsRotateAroundXAxis) ? mainDot.x : ((IsRotateAroundZAxis) ? GetVectorRotateXYOnAngle({ mainDot.x, mainDot.y, mainDot.z }, angle).x : mainDot.x * cos(fromDegreesToRad(angle)) + mainDot.z * sin(fromDegreesToRad(angle))),
        (IsRotateAroundXAxis) ? mainDot.y * cos(fromDegreesToRad(angle)) + mainDot.z * sin(fromDegreesToRad(angle)) : ((IsRotateAroundZAxis) ? GetVectorRotateXYOnAngle({ mainDot.x, mainDot.y, mainDot.z }, angle).y : mainDot.y),
        (IsRotateAroundXAxis) ? -mainDot.y * sin(fromDegreesToRad(angle)) + mainDot.z * cos(fromDegreesToRad(angle)) : ((IsRotateAroundZAxis) ? mainDot.z : -mainDot.x * sin(fromDegreesToRad(angle)) + mainDot.z * cos(fromDegreesToRad(angle))) };
}

#define IS_EQUAL(num1, num2) abs(num1) > num2 - 0.01f && abs(num1) < num2 + 0.01f
bool Parallelepiped::IsDotDisplayAfterFiguresDel(COORDS cameraNormalizeRay, COORDS cameraCoord, COORDS& nearCrossingRayAndFigureCoord, float farCrossingRayAndFigureValue, int& rayHitSurfaceNumber) const noexcept
{
    COORDS farCrossingRayAndFigureCoord = { cameraNormalizeRay.x * farCrossingRayAndFigureValue + cameraCoord.x, cameraNormalizeRay.y * farCrossingRayAndFigureValue + cameraCoord.y, cameraNormalizeRay.z * farCrossingRayAndFigureValue + cameraCoord.z };
    COORDS mainExtendedGroundDots[ParMainGroundDotsCount] = { extendedGroundParMainDot1, extendedGroundParMainDot2, extendedGroundParMainDot3, extendedGroundParMainDot4 };
    COORDS unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, unnecessaryShapeMainDot4, unnecessaryShapeMainDot5, unnecessaryShapeMainDot6;
    float surfaceValue1, reverseSurfaceValue1, surfaceValue2, reverseSurfaceValue2, surfaceValue3, reverseSurfaceValue3, surfaceValue4,
        reverseSurfaceValue4, surfaceValue5, reverseSurfaceValue5;

    for (size_t i = 0; i < ParMovingSurfacesCount; ++i)
    {
        float maxNonRotatedParWidthOrLength = (IsRotateAroundZAxis ? maxNonRotatedExtendedParWidth : maxNonRotatedExtendedParLenght);
        unnecessaryShapeMainDot1 = mainExtendedGroundDots[i];
        unnecessaryShapeMainDot2 = mainExtendedGroundDots[(i == 3) ? 0 : i + 1];
        unnecessaryShapeMainDot3 = { (IsRotateAroundXAxis) ? (length / 2) + extendValue : unnecessaryShapeMainDot1.x, (IsRotateAroundYAxis) ? (width / 2) + extendValue : unnecessaryShapeMainDot1.y, (IsRotateAroundZAxis) ? (height / 2) + extendValue : unnecessaryShapeMainDot1.z };
        unnecessaryShapeMainDot4 = { (IsRotateAroundXAxis) ? (length / 2) + extendValue : unnecessaryShapeMainDot2.x, (IsRotateAroundYAxis) ? (width / 2) + extendValue : unnecessaryShapeMainDot2.y, (IsRotateAroundZAxis) ? (height / 2) + extendValue : unnecessaryShapeMainDot2.z };
        unnecessaryShapeMainDot5 = { (IsRotateAroundXAxis) ? -(length / 2) - extendValue : ((IS_EQUAL(unnecessaryShapeMainDot1.x, maxNonRotatedExtendedParLenght)) ? unnecessaryShapeMainDot1.x : unnecessaryShapeMainDot2.x), (IsRotateAroundYAxis) ? -(width / 2) - extendValue : ((IS_EQUAL(unnecessaryShapeMainDot1.y, maxNonRotatedParWidthOrLength)) ? unnecessaryShapeMainDot1.y : unnecessaryShapeMainDot2.y), (IsRotateAroundZAxis) ? -(height / 2) - extendValue : ((IS_EQUAL(unnecessaryShapeMainDot1.z, maxNonRotatedExtendedParWidth)) ? unnecessaryShapeMainDot1.z : unnecessaryShapeMainDot2.z) };
        unnecessaryShapeMainDot6 = { (IsRotateAroundXAxis) ? (length / 2) + extendValue : unnecessaryShapeMainDot5.x, (IsRotateAroundYAxis) ? (width / 2) + extendValue : unnecessaryShapeMainDot5.y, (IsRotateAroundZAxis) ? (height / 2) + extendValue : unnecessaryShapeMainDot5.z };

        SurfacesValuesCalc(nearCrossingRayAndFigureCoord, unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, unnecessaryShapeMainDot4, unnecessaryShapeMainDot5, unnecessaryShapeMainDot6,
            surfaceValue1, reverseSurfaceValue1, surfaceValue2, reverseSurfaceValue2, surfaceValue3, reverseSurfaceValue3, surfaceValue4, reverseSurfaceValue4, surfaceValue5, reverseSurfaceValue5);

        if ((IsSign(surfaceValue1, reverseSurfaceValue1) && IsSign(surfaceValue2, reverseSurfaceValue2) && IsSign(surfaceValue3, reverseSurfaceValue3) && IsSign(surfaceValue4, reverseSurfaceValue4) && IsSign(surfaceValue5, reverseSurfaceValue5)))
        {
            SurfacesValuesCalc(farCrossingRayAndFigureCoord, unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, unnecessaryShapeMainDot4, unnecessaryShapeMainDot5, unnecessaryShapeMainDot6,
                surfaceValue1, reverseSurfaceValue1, surfaceValue2, reverseSurfaceValue2, surfaceValue3, reverseSurfaceValue3, surfaceValue4, reverseSurfaceValue4, surfaceValue5, reverseSurfaceValue5);

            if ((IsSign(surfaceValue1, reverseSurfaceValue1) && IsSign(surfaceValue2, reverseSurfaceValue2) && IsSign(surfaceValue3, reverseSurfaceValue3) && IsSign(surfaceValue4, reverseSurfaceValue4) && IsSign(surfaceValue5, reverseSurfaceValue5)))
            {
                return false;
            }

            COORDS farCrossingRayAndFigureExtendedCoord = { cameraNormalizeRay.x * (farCrossingRayAndFigureValue + 1) + cameraCoord.x, cameraNormalizeRay.y * (farCrossingRayAndFigureValue + 1) + cameraCoord.y, cameraNormalizeRay.z * (farCrossingRayAndFigureValue + 1) + cameraCoord.z };
            COORDS crossingSurfaceAndLineCoords = GetCrossingSurfaceAndLineCoords(unnecessaryShapeMainDot1, unnecessaryShapeMainDot2, unnecessaryShapeMainDot3, nearCrossingRayAndFigureCoord - centreCoord, farCrossingRayAndFigureExtendedCoord - centreCoord);

            nearCrossingRayAndFigureCoord = crossingSurfaceAndLineCoords + centreCoord;
            rayHitSurfaceNumber = i;

            return true;
        }
    }

    static const int ParFirstNonMovingSurfaceIndex = 4;
    static const int ParSecondNonMovingSurfaceIndex = 5;

    if (!(((IsRotateAroundXAxis) ? nearCrossingRayAndFigureCoord.x : ((IsRotateAroundZAxis) ? nearCrossingRayAndFigureCoord.z : nearCrossingRayAndFigureCoord.y)) >= ((IsRotateAroundXAxis) ? parRange.xStart : ((IsRotateAroundZAxis) ? parRange.zStart : parRange.yStart)) + extendValue))
    {
        rayHitSurfaceNumber = ParFirstNonMovingSurfaceIndex;
    }
    else if (!(((IsRotateAroundXAxis) ? nearCrossingRayAndFigureCoord.x : ((IsRotateAroundZAxis) ? nearCrossingRayAndFigureCoord.z : nearCrossingRayAndFigureCoord.y)) <= ((IsRotateAroundXAxis) ? parRange.xEnd : ((IsRotateAroundZAxis) ? parRange.zEnd : parRange.yEnd)) - extendValue))
    {
        rayHitSurfaceNumber = ParSecondNonMovingSurfaceIndex;
    }

    return true;
}