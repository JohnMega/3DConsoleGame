
#include <format>
#include <string>
#include <fstream>
#include "Circle.h"
#include "Preprocessor.h"

extern double tick;

COORDS Circle::CircleDotToLocalCoords(COORDS circleDot) const
{
	float x = dotDistance(localSpriteInfo.localCentreCoord, circleDot) * cos(fromDegreesToRad(GetAngleBetweenVectors(localSpriteInfo.localCentreCoord, circleDot, localSpriteInfo.localXVec)));
	float y = dotDistance(localSpriteInfo.localCentreCoord, circleDot) * sin(fromDegreesToRad(GetAngleBetweenVectors(localSpriteInfo.localCentreCoord, circleDot, localSpriteInfo.localXVec)));

	return { x, y, 0 };
}

Circle::Circle(COORDS centreCoord, COORDS normVec, const std::string& bmpName, float rad, int color, bool isCircleRender, float nextAnimFrameDelay)
	: centreCoord(centreCoord)
	, normVec(normVec)
	, rad(rad)
	, color(color)
	, isCircleRender(isCircleRender)
	, isCircleRotate(true)
	, isLimpid(false)
	, nextAnimFrameDelay(nextAnimFrameDelay)
{
	for (size_t i = 0; i < spritesInfo.size(); ++i)
	{
		if (bmpName == spritesInfo[i].textureName)
			currentSpriteIndex = i;
	}

	// missing textures
	if (currentSpriteIndex == -1) currentSpriteIndex = 0;

	UpdateTextureVars();

	objType = objectType::CIRCLE;
}

void Circle::TextureLoad(std::string textureName, int index)
{
	spritesInfo[index].textureName = textureName;
	spritesInfo[index].bitmap.emplace_back();
	spritesInfo[index].bmpPixelMatrix.emplace_back();

	while (true)
	{
		std::string spritesFrame = spritesInfo[index].textureName + std::format("{}", spritesInfo[index].frameCounter);

		std::ifstream in(FILE_EXTENSION(std::string(spritesFrame), EXTENSION_CAST(bmp)));
		if (!in.is_open())
		{
			if (spritesInfo[index].frameCounter == 0)
			{
				spritesInfo[index].frameCounter++;
				spritesInfo[index].bitmap.back().open(spritesInfo[index].textureName);
				spritesInfo[index].bmpPixelMatrix.back() = spritesInfo[index].bitmap.back().toPixelMatrix();
			}

			break;
		}
		
		spritesInfo[index].bitmap.back().open(FILE_EXTENSION(std::string(spritesFrame), EXTENSION_CAST(bmp)));
		spritesInfo[index].bmpPixelMatrix.back() = spritesInfo[index].bitmap.back().toPixelMatrix();

		spritesInfo[index].frameCounter++;
		spritesInfo[index].bitmap.emplace_back();
		spritesInfo[index].bmpPixelMatrix.emplace_back();
	}
}

void Circle::SetRenderMode(bool isCircleRenderMode) noexcept
{
	isCircleRender = isCircleRenderMode;
}

bool Circle::IsCircleRender() const noexcept
{
	return isCircleRender;
}

int& Circle::GetSpriteCurrentFrame() noexcept
{
	return localSpriteInfo.currentFrame;
}

int Circle::GetSpriteFrameCount() const noexcept
{
	return spritesInfo[currentSpriteIndex].frameCounter;
}

COORDS& Circle::SetCentreCoord() noexcept
{
	return centreCoord;
}

COORDS Circle::GetCentreCoord() const noexcept
{
	return centreCoord;
}

void Circle::SetLimpidMode(bool limpidMode) noexcept
{
	isLimpid = limpidMode;
}

bool Circle::GetLimpidMode() const noexcept
{
	return isLimpid;
}

void Circle::UpdateTextureVars()
{
	COORDS up = GetVectorRotateZOnAngle(normVec, 90);
	COORDS down = up * (-1);
	COORDS left = { up.y * normVec.z - up.z * normVec.y, up.z * normVec.x - up.x * normVec.z, up.x * normVec.y - up.y * normVec.x };
	COORDS right = left * (-1);

	surfaceDot1 = left + centreCoord;
	surfaceDot2 = up + centreCoord;

	localSpriteInfo.localCentreCoord = (right * rad + centreCoord) + up * rad;
	localSpriteInfo.localXVec = vectorNormalize(localSpriteInfo.localCentreCoord + left * rad, localSpriteInfo.localCentreCoord) + localSpriteInfo.localCentreCoord;
	localSpriteInfo.localYVec = vectorNormalize(localSpriteInfo.localCentreCoord + down * rad, localSpriteInfo.localCentreCoord) + localSpriteInfo.localCentreCoord;
}

COORDS Circle::GetNormVec() const noexcept
{
	return normVec;
}

void Circle::SetNormVec(COORDS dot)
{
	if (isCircleRotate)
		normVec = vectorNormalize(dot, centreCoord);

	UpdateTextureVars();
}

void Circle::SetCircleRotateMode(bool circleRotateMode)
{
	isCircleRotate = circleRotateMode;
}

bool Circle::GetCircleRotateMode() const noexcept
{
	return isCircleRotate;
}

COORDS Circle::isRayIntersectCircle(COORDS startRayDot, COORDS endRayDot)
{
	COORDS dotProjection = GetCrossingSurfaceAndLineCoords(centreCoord, surfaceDot1, surfaceDot2, normVec, startRayDot, endRayDot);

	if (dotDistance(centreCoord, dotProjection) <= rad)
		return dotProjection;
	else
		return { 0, 0, 0 };
}

bool Circle::isObjHasTexture() const noexcept
{
	return true;
}

std::string Circle::GetTextureName() const noexcept
{
	return spritesInfo[currentSpriteIndex].textureName;
}

void Circle::AnimationHandle()
{
	localSpriteInfo.tickSum += tick;

	if (localSpriteInfo.tickSum >= nextAnimFrameDelay)
	{
		localSpriteInfo.tickSum = 0;
		localSpriteInfo.currentFrame = (localSpriteInfo.currentFrame == spritesInfo[currentSpriteIndex].frameCounter - 1 ? 0 : localSpriteInfo.currentFrame + 1);
	}
}

void Circle::SpritesPreLoad(std::vector<std::string> sprites)
{
	static bool isFinish = false;
	
	if(!isFinish)
	for (size_t i = 0; i < sprites.size(); ++i)
	{
		spritesInfo.emplace_back();
		TextureLoad(sprites[i], i);
	}

	isFinish = true;
}

COORDS Circle::GetColor(COORDS circleDot) const
{
	COORDS newCircleDot = CircleDotToLocalCoords(circleDot);
	COORDS scalingNewCircleDot = { newCircleDot.x * (spritesInfo[currentSpriteIndex].bitmap[localSpriteInfo.currentFrame].wight / (rad * 2)), newCircleDot.y * (spritesInfo[currentSpriteIndex].bitmap[localSpriteInfo.currentFrame].height / (rad * 2)), newCircleDot.z };

	int line, column;

	try
	{
		line = scalingNewCircleDot.y > spritesInfo[currentSpriteIndex].bmpPixelMatrix[localSpriteInfo.currentFrame].size() || scalingNewCircleDot.y < 0 ? 0 : (int)scalingNewCircleDot.y;
		column = scalingNewCircleDot.x > spritesInfo[currentSpriteIndex].bmpPixelMatrix[localSpriteInfo.currentFrame].at(line).size() || scalingNewCircleDot.x < 0 ? 0 : (int)scalingNewCircleDot.x;
		spritesInfo[currentSpriteIndex].bmpPixelMatrix[localSpriteInfo.currentFrame].at(line).at(column);
	}
	catch (...)
	{
		line = column = 0;
	}

	BmpPixel currentPolygonRGBColor = spritesInfo[currentSpriteIndex].bmpPixelMatrix[localSpriteInfo.currentFrame][line][column];
	return { (float)currentPolygonRGBColor.red, (float)currentPolygonRGBColor.green, (float)currentPolygonRGBColor.blue };
}