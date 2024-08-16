#pragma once

#include <cmath>

const float PI = 3.14;
const float EXP = 2.7;

struct COORDS
{
    float x;
    float y;
    float z;

    void operator+=(COORDS coord2)
    {
        x += coord2.x;
        y += coord2.y;
        z += coord2.z;
    }

    void operator-=(COORDS coord2)
    {
        x -= coord2.x;
        y -= coord2.y;
        z -= coord2.z;
    }

    bool operator==(COORDS coord2) const
    {
        return ((x == coord2.x && y == coord2.y && z == coord2.z) ? true : false);
    }

    COORDS operator+(COORDS coord2) const
    {
        return { x + coord2.x, y + coord2.y, z + coord2.z };
    }

    COORDS operator-(COORDS coord2) const
    {
        return { x - coord2.x, y - coord2.y, z - coord2.z };
    }

    COORDS operator*(COORDS coord2) const
    {
        return { x * coord2.x, y * coord2.y, z * coord2.z };
    }

    COORDS operator/(COORDS coord2) const
    {
        return { x / coord2.x, y / coord2.y, z / coord2.z };
    }

    COORDS operator+(float value)
    {
        return { x + value, y + value, z + value };
    }

    COORDS operator-(float value)
    {
        return { x - value, y - value, z - value };
    }

    COORDS operator*(float value)
    {
        return { x * value, y * value, z * value };
    }

    COORDS operator/(float value)
    {
        return { x / value, y / value, z / value };
    }
};

struct Range
{
    float xStart;
    float xEnd;
    float yStart;
    float yEnd;
    float zStart;
    float zEnd;

    bool operator==(Range range2) const
    {
        return ((xStart == range2.xStart && xEnd == range2.xEnd && yStart == range2.yStart && yEnd == range2.yEnd &&
            zStart == range2.zStart && zEnd == range2.zEnd) ? true : false);
    }
};

float dotDistance(COORDS firstDot, COORDS secondDot);

float dotProduct(COORDS vec1, COORDS cameraNormalizeRay) noexcept;

float fromRadToDegrees(float radians) noexcept;

float fromDegreesToRad(float degrees) noexcept;

float GetAngleBetweenVectors(COORDS generalDotOfVect, COORDS firstDotOfVect, COORDS secondDotOfVect);

float mixedProduct(COORDS dot1, COORDS dot2, COORDS dot3);

float crossProduct(COORDS dot1, COORDS dot2);

COORDS GetSurfaceNormVec(COORDS surfaceDot1, COORDS surfaceDot2, COORDS surfaceDot3);

COORDS ToCartesianFromSphereCoords(COORDS dot);

COORDS ToSphereFromCartesianCoords(COORDS dot);

COORDS crossProductVec(COORDS dot1, COORDS dot2);

COORDS VecZAxisRotation(COORDS vec, float angle);

COORDS VecXAxisRotation(COORDS vec, float angle);

COORDS VecYAxisRotation(COORDS vec, float angle);

COORDS GetVectorRotateXYOnAngle(COORDS vecCoord, float angle) noexcept;

COORDS GetVectorRotateZOnAngle(COORDS vecCoord, float angle);

COORDS dotProjectionOnSurface(COORDS surfaceDot1, COORDS surfaceDot2, COORDS mainDot) noexcept;

COORDS vectorNormalize(COORDS vecEnd, COORDS vecStart);

COORDS GetCrossingSurfaceAndLineCoords(COORDS surfaceDot1, COORDS surfaceDot2, COORDS surfaceDot3, COORDS lineStartCoord, COORDS lineEndCoord) noexcept;

COORDS GetCrossingSurfaceAndLineCoords(COORDS surfaceDot1, COORDS surfaceDot2, COORDS surfaceDot3, COORDS normVec, COORDS lineStartCoord, COORDS lineEndCoord) noexcept;

void MotionCubic(float DesiredPosition, float Interval, float* Position, float* Rate);

void MotionLinear(float DesiredPosition, float Interval, float* Position);

float GetSurfaceValue(COORDS surfaceDot1, COORDS surfaceDot2, COORDS surfaceDot3, COORDS checkedDot) noexcept;

float IsDotCrossTriangle(COORDS surfaceDot1, COORDS surfaceDot2, COORDS surfaceDot3, COORDS orig, COORDS dir, float minDotAndCameraDistValue);

bool IsRayHitFigureSurfaceDot(COORDS surfaceDot1, COORDS surfaceDot2, COORDS surfaceDot3,
    COORDS rayStartCoord, COORDS checkedSurfaceDot, COORDS figureCentreCoord);

bool IsSign(float firstValue, float secondValue) noexcept;

float minNumb(float num1, float num2, float num3) noexcept;

float maxNumb(float num1, float num2, float num3) noexcept;

void doIntersect(COORDS p1, COORDS q1, COORDS p2, COORDS q2, COORDS& intersection);