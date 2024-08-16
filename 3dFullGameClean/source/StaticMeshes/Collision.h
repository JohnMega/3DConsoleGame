#pragma once

#include "Maths.h"

class Collision
{
protected:
	virtual COORDS DownDotCollisionHandle(COORDS mainDot, COORDS finalSurfaceDot1, COORDS finalSurfaceDot2, COORDS finalSurfaceDot3)
	{
		return { NULL, NULL, NULL };
	}

	virtual COORDS UpDotCollisionHandle(COORDS mainDot, COORDS finalSurfaceDot1, COORDS finalSurfaceDot2, COORDS finalSurfaceDot3)
	{
		return { NULL, NULL, NULL };
	}

public:
	virtual COORDS CollisionHandle(COORDS mainDotDir, COORDS mainDot) = 0;
};