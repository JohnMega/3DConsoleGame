#pragma once

#include "Parallelepiped.h"

class InvisibleParallelepiped : public Parallelepiped
{
public:
	InvisibleParallelepiped(float length, float width, float height, COORDS centreCoord, bool IsRotateAroundZAxis, bool IsRotateAroundXAxis,
		bool IsRotateAroundYAxis, int color, objectType objType) noexcept : Parallelepiped(length, width, height, centreCoord, IsRotateAroundZAxis, IsRotateAroundXAxis
		, IsRotateAroundYAxis, color)
	{
		this->objType = objType;
	}
};