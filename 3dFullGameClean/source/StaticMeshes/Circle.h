#pragma once

#include <iostream>
#include "bitmap.h"
#include "Maths.h"
#include "StaticMesh.h"

class Circle : public StaticMesh
{
	struct GlobalSpritesInfo
	{
		std::vector<Bitmap> bitmap;
		std::vector<PixelMatrix> bmpPixelMatrix;
		std::string textureName;
		int frameCounter = 0;
	};

	struct LocalSpriteInfo
	{
		int currentFrame = 0;
		float tickSum = 0;
		COORDS localCentreCoord;
		COORDS localXVec, localYVec;
	};

private:
	static inline std::vector<GlobalSpritesInfo> spritesInfo;
	float nextAnimFrameDelay = 0.05f;
	int currentSpriteIndex = -1;

	LocalSpriteInfo localSpriteInfo;

	COORDS normVec;
	COORDS centreCoord;
	COORDS surfaceDot1, surfaceDot2;

	float rad;
	int color;
	bool isCircleRender;
	bool isCircleRotate;
	bool isLimpid;

private:
	COORDS CircleDotToLocalCoords(COORDS circleDot) const;

	void UpdateTextureVars();

	static void TextureLoad(std::string textureName, int index);

public:
	Circle(COORDS centreCoord, COORDS normVec, const std::string& bmpName, float rad, int color, bool isCircleRender = true, float nextAnimFrameDelay = 0.05f);

	static void SpritesPreLoad(std::vector<std::string> sprites);

	void SetRenderMode(bool isCircleRenderMode) noexcept;

	bool IsCircleRender() const noexcept;

	int& GetSpriteCurrentFrame() noexcept;

	int GetSpriteFrameCount() const noexcept;

	COORDS& SetCentreCoord() noexcept override;

	COORDS GetCentreCoord() const noexcept override;

	void SetLimpidMode(bool limpidMode) noexcept;

	bool GetLimpidMode() const noexcept;

	COORDS GetNormVec() const noexcept;

	void SetNormVec(COORDS dot);

	void SetCircleRotateMode(bool circleRotateMode);

	bool GetCircleRotateMode() const noexcept;

	COORDS isRayIntersectCircle(COORDS startRayDot, COORDS endRayDot);

	bool isObjHasTexture() const noexcept override;

	std::string GetTextureName() const noexcept;

	void AnimationHandle();

	COORDS GetColor(COORDS circleDot) const override;
};