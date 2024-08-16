
#include <iostream>
#include <Windows.h>
#include <format>
#include <fstream>
#include "FantomModel.h"
#include "Preprocessor.h"
#include "ObjFileParse.h"
#include "Triangle.h"
#include "ACameraActor.h"

extern double tick;

FantomModel::FantomModel()
	: currentCentreCoord{ 0,0,0 }
	, oldCentreCoord{ 0,0,0 }
	, currAnimPartIndex(-1)
{
	objType = objectType::MODEL;
}

FantomModel::FantomModel(const FantomModel& other)
{
	currAnimPartIndex = 0;
	polygons.emplace_back();
	polygons[currAnimPartIndex].emplace_back();
	animInfo.push_back({ .animationDir = {1,0,0}, .tickSum = 0.0, .frameCounter = 0, .currentFrame = 0 , .isAnimated = true });
	polygons[0][0] = other.polygons[other.currAnimPartIndex][other.animInfo[other.currAnimPartIndex].currentFrame];
}

void FantomModel::AddNewModelSlot()
{
	polygons.emplace_back();
	polygons[++currAnimPartIndex].emplace_back();
	animInfo.push_back({ .animationDir = {1,0,0}, .tickSum = 0.0, .frameCounter = 0, .currentFrame = 0 , .isAnimated = false });
}

int FantomModel::GetCurrentAnimPartIndex()
{
	return currAnimPartIndex;
}

void FantomModel::SetCurrentAnimPartIndex(int newCurrAnimPartIndex)
{
	if (newCurrAnimPartIndex != currAnimPartIndex)
	{
		animInfo[currAnimPartIndex].currentFrame = 0;
		currAnimPartIndex = newCurrAnimPartIndex;

		COORDS tempCentreCoord = currentCentreCoord;
		CentreCoordCalc();
		SetCentreCoord() = tempCentreCoord;
		UpdateRange();
	}
}

Range FantomModel::GetRange() const noexcept
{
	return modelRange;
}

COORDS FantomModel::GetCentreCoord() const noexcept
{
	return currentCentreCoord;
}

COORDS FantomModel::GetOldCentreCoord() const noexcept
{
	return oldCentreCoord;
}

COORDS& FantomModel::SetCentreCoord() noexcept
{
	oldCentreCoord = currentCentreCoord;
	return currentCentreCoord;
}

FantomAnimationInfo FantomModel::GetAnimationInfo() const noexcept
{
	return animInfo[currAnimPartIndex];
}

FantomAnimationInfo& FantomModel::SetAnimationInfo() noexcept
{
	return animInfo[currAnimPartIndex];
}

std::string& FantomModel::SetFileName() noexcept
{
	return fileName;
}

void FantomModel::AddNewAnimationPart(const std::string& fileName)
{
	std::string objFileNameWithExtensions = "Models/";
	objFileNameWithExtensions += fileName;

	polygons.emplace_back();
	polygons[++currAnimPartIndex].emplace_back();
	animInfo.push_back({ .animationDir = {1,0,0}, .tickSum = 0.0, .frameCounter = 0, .currentFrame = 0 , .isAnimated = true });

	while (true)
	{
		std::string modelAnimFrame = objFileNameWithExtensions + std::format("{:04}", GetAnimationInfo().frameCounter);

		std::ifstream in(FILE_EXTENSION(std::string(modelAnimFrame), EXTENSION_CAST(obj)));
		if (!in.is_open())
		{
			//UpdateRange();
			break;
		}

		SetAnimationInfo().frameCounter++;
		ObjParseInfo objParseInfo = ObjParse(FILE_EXTENSION(std::string(modelAnimFrame), EXTENSION_CAST(obj)).c_str(), modelAnimFrame.c_str());

		for (size_t i = 0; i < objParseInfo.faceIndex.size(); ++i)
		{
			if (GetAnimationInfo().frameCounter > 1)
				GetAnimModelPolygons().emplace_back();

			GetAnimModelPolygons()[GetAnimationInfo().frameCounter - 1].push_back(new Triangle(objParseInfo.vertices[objParseInfo.faceIndex[i][0].x], objParseInfo.vertices[objParseInfo.faceIndex[i][0].y], objParseInfo.vertices[objParseInfo.faceIndex[i][0].z], (Model*)this, objParseInfo.faceIndex[i][1]));
			((Triangle*)GetAnimModelPolygons()[GetAnimationInfo().frameCounter - 1].back())->SetTextureParams(objParseInfo.facesTextureNames[i], objParseInfo.textures[objParseInfo.faceIndex[i][1].x], objParseInfo.textures[objParseInfo.faceIndex[i][1].y], objParseInfo.textures[objParseInfo.faceIndex[i][1].z]);
		}

		size_t verticesSize = objParseInfo.vertices.size();
		size_t facesSize = objParseInfo.faceIndex.size();
		size_t texturesSize = objParseInfo.textures.size();
		size_t texturesNamesSize = objParseInfo.facesTextureNames.size();
		for (size_t i = 0; i < verticesSize; ++i) objParseInfo.vertices.pop_back();
		for (size_t i = 0; i < facesSize; ++i) objParseInfo.faceIndex.pop_back();
		for (size_t i = 0; i < texturesSize; ++i) objParseInfo.textures.pop_back();
		for (size_t i = 0; i < texturesNamesSize; ++i) objParseInfo.facesTextureNames.pop_back();
	}

	COORDS tempCentreCoord = currentCentreCoord;
	SetCentreCoord() = tempCentreCoord;
	UpdateRange();
}

void FantomModel::AnimationHandle() noexcept
{
	if (animInfo[currAnimPartIndex].isAnimated)
	{
		animInfo[currAnimPartIndex].tickSum += tick;

		if (animInfo[currAnimPartIndex].tickSum >= (polygons.size() > 1 ? nextAnimFrameDelay / 2.0f : nextAnimFrameDelay))
		{
			animInfo[currAnimPartIndex].tickSum = 0;
			animInfo[currAnimPartIndex].currentFrame = (animInfo[currAnimPartIndex].currentFrame + 1 == animInfo[currAnimPartIndex].frameCounter - 1 ? 0 : animInfo[currAnimPartIndex].currentFrame + 1);
		}
	}
}

#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

void FantomModel::CentreCoordCalc()
{
	Range modelPar = { .xStart = 10000, .xEnd = -10000, .yStart = 10000, .yEnd = -10000, .zStart = 10000, .zEnd = -10000 };
	for (int i = 0; i < (animInfo[currAnimPartIndex].frameCounter == 0 ? 1 : polygons[currAnimPartIndex].size() / polygons[currAnimPartIndex][0].size()); ++i)
		for (int j = 0; j < polygons[currAnimPartIndex][i].size(); ++j)
		{
			modelPar.xStart = std::min(polygons[currAnimPartIndex][i][j]->GetCentreCoord().x, modelPar.xStart);
			modelPar.xEnd = std::max(polygons[currAnimPartIndex][i][j]->GetCentreCoord().x, modelPar.xEnd);
			modelPar.yStart = std::min(polygons[currAnimPartIndex][i][j]->GetCentreCoord().y, modelPar.yStart);
			modelPar.yEnd = std::max(polygons[currAnimPartIndex][i][j]->GetCentreCoord().y, modelPar.yEnd);
			modelPar.zStart = std::min(polygons[currAnimPartIndex][i][j]->GetCentreCoord().z, modelPar.zStart);
			modelPar.zEnd = std::max(polygons[currAnimPartIndex][i][j]->GetCentreCoord().z, modelPar.zEnd);
		}

	if(currAnimPartIndex != JUMP_ANIM_MODEL_INDEX) modelRange = modelPar;
	//modelRange = modelPar;

	currentCentreCoord = oldCentreCoord = { (modelPar.xStart + modelPar.xEnd) / 2, (modelPar.yStart + modelPar.yEnd) / 2, (modelPar.zStart + modelPar.zEnd) / 2 };
}

#pragma pop_macro("min")
#pragma pop_macro("max")

void FantomModel::UpdateRange(size_t flag) noexcept
{
	if (!currentCentreCoord.x && !currentCentreCoord.y && !currentCentreCoord.z)
		CentreCoordCalc();
}

void FantomModel::Rotation(float angle, size_t flag)
{
	CentreCoordCalc();
}

void FantomModel::Rotation(COORDS camDir)
{
	float p1 = camDir.y - (camDir.x * animInfo[currAnimPartIndex].animationDir.y) / animInfo[currAnimPartIndex].animationDir.x;
	float angle = fromRadToDegrees(asin(p1 / (pow(animInfo[currAnimPartIndex].animationDir.y, 2) / animInfo[currAnimPartIndex].animationDir.x + animInfo[currAnimPartIndex].animationDir.x)));

	if (std::isnan(angle)) return;

	animInfo[currAnimPartIndex].animationDir = GetVectorRotateXYOnAngle(animInfo[currAnimPartIndex].animationDir, angle);
}

AnimationPart& FantomModel::GetAnimModelPolygons() noexcept
{
	return polygons[currAnimPartIndex];
}

std::vector<StaticMesh*>& FantomModel::GetModelPolygons() noexcept
{
	return polygons[currAnimPartIndex][animInfo[currAnimPartIndex].currentFrame];
}