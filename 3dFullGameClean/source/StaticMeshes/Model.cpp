
#include <iostream>
#include <Windows.h>
#include <mutex>
#include <format>
#include <fstream>
#include "Model.h"
#include "Preprocessor.h"
#include "ObjFileParse.h"
#include "Triangle.h"
#include "ACameraActor.h"

extern double tick;

Model::Model()
	: currentCentreCoord{ 0,0,0 }
	, oldCentreCoord{ 0,0,0 }
	, currAnimPartIndex(-1)
{
	objType = objectType::MODEL;
}

Model::Model(const Model& other)
{
	currAnimPartIndex = 0;
	polygons.emplace_back();
	polygons[currAnimPartIndex].emplace_back();
	animInfo.push_back({ .animationDir = {1,0,0}, .tickSum = 0.0, .frameCounter = 0, .currentFrame = 0 , .isAnimated = true });
	polygons[0][0] = other.polygons[other.currAnimPartIndex][other.animInfo[other.currAnimPartIndex].currentFrame];
}

void Model::AddNewModelSlot()
{
	polygons.emplace_back();
	polygons[++currAnimPartIndex].emplace_back();
	animInfo.push_back({ .animationDir = {1,0,0}, .tickSum = 0.0, .frameCounter = 0, .currentFrame = 0 , .isAnimated = false });
}

int Model::GetCurrentAnimPartIndex()
{
	return currAnimPartIndex;
}

void Model::SetCurrentAnimPartIndex(int newCurrAnimPartIndex)
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

void Model::SetScale(float newScale)
{
	COORDS newSurfaceDot1, newSurfaceDot2, newSurfaceDot3;
	COORDS polygonCentre;
	for (int i = 0; i < (animInfo[currAnimPartIndex].frameCounter == 0 ? 1 : polygons[currAnimPartIndex].size() / polygons[currAnimPartIndex][0].size()); ++i)
		for (int j = 0; j < polygons[currAnimPartIndex][i].size(); ++j)
		{
			polygonCentre = polygons[currAnimPartIndex][i][j]->GetCentreCoord();

			newSurfaceDot1 = currentCentreCoord + vectorNormalize(((Triangle*)polygons[currAnimPartIndex][i][j])->GetSurfaceDot1(), currentCentreCoord) * dotDistance(((Triangle*)polygons[currAnimPartIndex][i][j])->GetSurfaceDot1(), currentCentreCoord) * (1 + newScale);
			newSurfaceDot2 = currentCentreCoord + vectorNormalize(((Triangle*)polygons[currAnimPartIndex][i][j])->GetSurfaceDot2(), currentCentreCoord) * dotDistance(((Triangle*)polygons[currAnimPartIndex][i][j])->GetSurfaceDot2(), currentCentreCoord) * (1 + newScale);
			newSurfaceDot3 = currentCentreCoord + vectorNormalize(((Triangle*)polygons[currAnimPartIndex][i][j])->GetSurfaceDot3(), currentCentreCoord) * dotDistance(((Triangle*)polygons[currAnimPartIndex][i][j])->GetSurfaceDot3(), currentCentreCoord) * (1 + newScale);

			((Triangle*)polygons[currAnimPartIndex][i][j])->SetNewSurfaceDots(newSurfaceDot1, newSurfaceDot2, newSurfaceDot3);

		}

	CalcModelRange();
}

Range Model::GetModelRange() const noexcept
{
	return modelRange;
}

bool Model::isObjHasTexture() const noexcept
{
	return true;
}

COORDS Model::GetCentreCoord() const noexcept
{
	return currentCentreCoord;
}

COORDS Model::GetOldCentreCoord() const noexcept
{
	return oldCentreCoord;
}

COORDS& Model::SetCentreCoord() noexcept
{
	oldCentreCoord = currentCentreCoord;
	return currentCentreCoord;
}

AnimationInfo Model::GetAnimationInfo() const noexcept
{
	return animInfo[currAnimPartIndex];
}

AnimationInfo& Model::SetAnimationInfo() noexcept
{
	return animInfo[currAnimPartIndex];
}

std::string& Model::SetFileName() noexcept
{
	return fileName;
}

void Model::AddNewAnimationPart(const std::string& fileName)
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

			GetAnimModelPolygons()[GetAnimationInfo().frameCounter - 1].push_back(new Triangle(objParseInfo.vertices[objParseInfo.faceIndex[i][0].x], objParseInfo.vertices[objParseInfo.faceIndex[i][0].y], objParseInfo.vertices[objParseInfo.faceIndex[i][0].z], this, objParseInfo.faceIndex[i][1]));
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

void Model::AnimationHandle() noexcept
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

void Model::CalcModelRange()
{
	Range modelPar = { .xStart = 10000, .xEnd = -10000, .yStart = 10000, .yEnd = -10000, .zStart = 10000, .zEnd = -10000 };
	for (int i = 0; i < (animInfo[currAnimPartIndex].frameCounter == 0 ? 1 : polygons[currAnimPartIndex].size() / polygons[currAnimPartIndex][0].size()); ++i)
		for (int j = 0; j < polygons[currAnimPartIndex][i].size(); ++j)
		{
			float minX, maxX, minY, maxY, minZ, maxZ;
			Triangle* polygon = (Triangle*)polygons[currAnimPartIndex][i][j];
			minX = std::min(polygon->GetSurfaceDot1().x, std::min(polygon->GetSurfaceDot2().x, polygon->GetSurfaceDot3().x));
			maxX = std::max(polygon->GetSurfaceDot1().x, std::max(polygon->GetSurfaceDot2().x, polygon->GetSurfaceDot3().x));
			minY = std::min(polygon->GetSurfaceDot1().y, std::min(polygon->GetSurfaceDot2().y, polygon->GetSurfaceDot3().y));
			maxY = std::max(polygon->GetSurfaceDot1().y, std::max(polygon->GetSurfaceDot2().y, polygon->GetSurfaceDot3().y));
			minZ = std::min(polygon->GetSurfaceDot1().z, std::min(polygon->GetSurfaceDot2().z, polygon->GetSurfaceDot3().z));
			maxZ = std::max(polygon->GetSurfaceDot1().z, std::max(polygon->GetSurfaceDot2().z, polygon->GetSurfaceDot3().z));

			modelPar.xStart = std::min(minX, modelPar.xStart);
			modelPar.xEnd = std::max(maxX, modelPar.xEnd);
			modelPar.yStart = std::min(minY, modelPar.yStart);
			modelPar.yEnd = std::max(maxY, modelPar.yEnd);
			modelPar.zStart = std::min(minZ, modelPar.zStart);
			modelPar.zEnd = std::max(maxZ, modelPar.zEnd);
		}

	modelRange = modelPar;
}

void Model::CentreCoordCalc()
{
	CalcModelRange();

	currentCentreCoord = oldCentreCoord = { (modelRange.xStart + modelRange.xEnd) / 2, (modelRange.yStart + modelRange.yEnd) / 2, (modelRange.zStart + modelRange.zEnd) / 2 };
}

#pragma pop_macro("min")
#pragma pop_macro("max")

void Model::UpdateRange(size_t flag) noexcept
{
	if (!currentCentreCoord.x && !currentCentreCoord.y && !currentCentreCoord.z)
		CentreCoordCalc();

	for (int i = 0; i < (animInfo[currAnimPartIndex].frameCounter == 0 ? 1 : polygons[currAnimPartIndex].size() / polygons[currAnimPartIndex][0].size()); ++i)
		for (int j = 0; j < polygons[currAnimPartIndex][i].size(); ++j)
		{
			polygons[currAnimPartIndex][i][j]->UpdateRange();
		}

	CalcModelRange();
}

void Model::Rotation(float angle, size_t flag)
{
	for (int i = 0; i < (animInfo[currAnimPartIndex].frameCounter == 0 ? 1 : polygons[currAnimPartIndex].size() / polygons[currAnimPartIndex][0].size()); ++i)
		for (int j = 0; j < polygons[currAnimPartIndex][i].size(); ++j)
			polygons[currAnimPartIndex][i][j]->Rotation(angle, flag);

	CentreCoordCalc();
}

void Model::Rotation(COORDS camDir)
{
	float p1 = camDir.y - (camDir.x * animInfo[currAnimPartIndex].animationDir.y) / animInfo[currAnimPartIndex].animationDir.x;
	float angle = fromRadToDegrees(asin(p1 / (pow(animInfo[currAnimPartIndex].animationDir.y, 2) / animInfo[currAnimPartIndex].animationDir.x + animInfo[currAnimPartIndex].animationDir.x)));

	if (std::isnan(angle)) return;

	for (int i = 0; i < (animInfo[currAnimPartIndex].frameCounter == 0 ? 1 : polygons[currAnimPartIndex].size() / polygons[currAnimPartIndex][0].size()); ++i)
		for (int j = 0; j < polygons[currAnimPartIndex][i].size(); ++j)
			polygons[currAnimPartIndex][i][j]->Rotation(angle, Z_AXIS_ROTATION_KEY);

	animInfo[currAnimPartIndex].animationDir = GetVectorRotateXYOnAngle(animInfo[currAnimPartIndex].animationDir, angle);
}

void Model::RotationAllAnimations(float angle, size_t flag)
{
	for(int z = 0; z < polygons.size(); ++z)
	for (int i = 0; i < (animInfo[z].frameCounter == 0 ? 1 : polygons[z].size() / polygons[z][0].size()); ++i)
		for (int j = 0; j < polygons[z][i].size(); ++j)
			polygons[z][i][j]->Rotation(angle, flag);

	CentreCoordCalc();
}

AnimationPart& Model::GetAnimModelPolygons() noexcept
{
	return polygons[currAnimPartIndex];
}

std::vector<StaticMesh*>& Model::GetModelPolygons() noexcept
{
	return polygons[currAnimPartIndex][animInfo[currAnimPartIndex].currentFrame];
}

std::vector<AnimationPart> Model::GetAllAnimModels() const noexcept
{
	return polygons;
}