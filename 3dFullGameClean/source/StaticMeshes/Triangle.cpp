#include <iostream>
#include <chrono>
#include <mutex>
#include <Windows.h>
#include "Triangle.h"

Triangle::Triangle(COORDS dot1, COORDS dot2, COORDS dot3, Model* ownerModel, COORDS rgbColor)
	: surfaceDot1(dot1)
	, surfaceDot2(dot2)
	, surfaceDot3(dot3)
	, ownerModel(ownerModel)
	, rgbColor(rgbColor)
{
	// Normal vec calc
	float normVecX, normVecY, normVecZ;

	normVecX = (surfaceDot2.y - surfaceDot1.y) * (surfaceDot3.z - surfaceDot1.z) - (surfaceDot3.y - surfaceDot1.y) * (surfaceDot2.z - surfaceDot1.z);
	normVecY = (surfaceDot2.z - surfaceDot1.z) * (surfaceDot3.x - surfaceDot1.x) - (surfaceDot3.z - surfaceDot1.z) * (surfaceDot2.x - surfaceDot1.x);
	normVecZ = (surfaceDot2.x - surfaceDot1.x) * (surfaceDot3.y - surfaceDot1.y) - (surfaceDot3.x - surfaceDot1.x) * (surfaceDot2.y - surfaceDot1.y);

	normVec = vectorNormalize({ normVecX, normVecY, normVecZ }, { 0,0,0 });

	up = (normVec.x == 0 && normVec.y == 0 ? COORDS{ -normVec.z, 0,0 } : GetVectorRotateZOnAngle(normVec, 90));
	down = up * (-1);
	left = { up.y * normVec.z - up.z * normVec.y, up.z * normVec.x - up.x * normVec.z, up.x * normVec.y - up.y * normVec.x };
	right = left * (-1);

	CentreCoordCalc();

	objType = objectType::TRIANGLE;
}

void Triangle::PreLoadTextures(std::vector<std::string> preLoadInfo)
{
	for (size_t i = 0; i < preLoadInfo.size(); ++i)
	{
		preLoadTextures.push_back(TexturesInfo{ .textureName = preLoadInfo[i] });
		preLoadTextures[i].bitmap.open(preLoadInfo[i]);
		preLoadTextures[i].pixelMatrix = preLoadTextures[i].bitmap.toPixelMatrix();
	}
}

void Triangle::ZAxisRotation(float angle)
{
	COORDS newSurfaceDot1 = surfaceDot1 - ownerModel->GetCentreCoord();
	COORDS newSurfaceDot2 = surfaceDot2 - ownerModel->GetCentreCoord();
	COORDS newSurfaceDot3 = surfaceDot3 - ownerModel->GetCentreCoord();

	surfaceDot1 = VecZAxisRotation(newSurfaceDot1, angle) + ownerModel->GetCentreCoord();
	surfaceDot2 = VecZAxisRotation(newSurfaceDot2, angle) + ownerModel->GetCentreCoord();
	surfaceDot3 = VecZAxisRotation(newSurfaceDot3, angle) + ownerModel->GetCentreCoord();

	up = VecZAxisRotation(up, angle);
	down = VecZAxisRotation(down, angle);
	left = VecZAxisRotation(left, angle);
	right = VecZAxisRotation(right, angle);
}

void Triangle::XAxisRotation(float angle)
{
	COORDS newSurfaceDot1 = surfaceDot1 - ownerModel->GetCentreCoord();
	COORDS newSurfaceDot2 = surfaceDot2 - ownerModel->GetCentreCoord();
	COORDS newSurfaceDot3 = surfaceDot3 - ownerModel->GetCentreCoord();

	surfaceDot1 = VecXAxisRotation(newSurfaceDot1, angle) + ownerModel->GetCentreCoord();
	surfaceDot2 = VecXAxisRotation(newSurfaceDot2, angle) + ownerModel->GetCentreCoord();
	surfaceDot3 = VecXAxisRotation(newSurfaceDot3, angle) + ownerModel->GetCentreCoord();

	up = VecXAxisRotation(up, angle);
	down = VecXAxisRotation(down, angle);
	left = VecXAxisRotation(left, angle);
	right = VecXAxisRotation(right, angle);
}

void Triangle::YAxisRotation(float angle)
{
	COORDS newSurfaceDot1 = surfaceDot1 - ownerModel->GetCentreCoord();
	COORDS newSurfaceDot2 = surfaceDot2 - ownerModel->GetCentreCoord();
	COORDS newSurfaceDot3 = surfaceDot3 - ownerModel->GetCentreCoord();

	surfaceDot1 = VecYAxisRotation(newSurfaceDot1, angle) + ownerModel->GetCentreCoord();
	surfaceDot2 = VecYAxisRotation(newSurfaceDot2, angle) + ownerModel->GetCentreCoord();
	surfaceDot3 = VecYAxisRotation(newSurfaceDot3, angle) + ownerModel->GetCentreCoord();

	up = VecYAxisRotation(up, angle);
	down = VecYAxisRotation(down, angle);
	left = VecYAxisRotation(left, angle);
	right = VecYAxisRotation(right, angle);
}

void Triangle::CentreCoordCalc()
{
	float a = dotDistance(surfaceDot1, surfaceDot2);
	float b = dotDistance(surfaceDot1, surfaceDot3);
	float c = dotDistance(surfaceDot2, surfaceDot3);

	float medLen = sqrt((2 * a * a + 2 * b * b - c * c) / (float)4);
	float lenToCentre = (2 / (float)3) * medLen;

	COORDS ser = { (surfaceDot2.x + surfaceDot3.x) / 2, (surfaceDot2.y + surfaceDot3.y) / 2, (surfaceDot2.z + surfaceDot3.z) / 2 };
	COORDS normToCentreVec = vectorNormalize(ser, surfaceDot1);

	centreCoord = { normToCentreVec.x * lenToCentre + surfaceDot1.x, normToCentreVec.y * lenToCentre + surfaceDot1.y, normToCentreVec.z * lenToCentre + surfaceDot1.z };
}

COORDS Triangle::GetSurfaceDot1() const noexcept
{
	return surfaceDot1;
}
COORDS Triangle::GetSurfaceDot2() const noexcept
{
	return surfaceDot2;
}
COORDS Triangle::GetSurfaceDot3() const noexcept
{
	return surfaceDot3;
}

COORDS Triangle::GetTriangleNormVec()
{
	return normVec;
}

COORDS Triangle::GetCentreCoord() const noexcept
{
	return centreCoord;
}

void Triangle::UpdateRange(size_t flag) noexcept
{
	surfaceDot1 -= ownerModel->GetOldCentreCoord();
	surfaceDot2 -= ownerModel->GetOldCentreCoord();
	surfaceDot3 -= ownerModel->GetOldCentreCoord();

	surfaceDot1 += ownerModel->GetCentreCoord();
	surfaceDot2 += ownerModel->GetCentreCoord();
	surfaceDot3 += ownerModel->GetCentreCoord();

	CentreCoordCalc();

	localCentreCoord = surfaceDot1;
	localXVec = localCentreCoord + left;
	localYVec = localCentreCoord + down;

	//localSurfaceDot2 = toLocalTriangleCoords(surfaceDot2);
	//localSurfaceDot3 = toLocalTriangleCoords(surfaceDot3);
}

void Triangle::SetNewSurfaceDots(COORDS newSurfaceDot1, COORDS newSurfaceDot2, COORDS newSurfaceDot3)
{
	surfaceDot1 = newSurfaceDot1;
	surfaceDot2 = newSurfaceDot2;
	surfaceDot3 = newSurfaceDot3;

	//CentreCoordCalc();

	localCentreCoord = surfaceDot1;
	localXVec = localCentreCoord + left;
	localYVec = localCentreCoord + down;

	localSurfaceDot2 = toLocalTriangleCoords(surfaceDot2);
	localSurfaceDot3 = toLocalTriangleCoords(surfaceDot3);

	TextureCalcVarsInit();
}

void Triangle::Rotation(float angle, size_t flag)
{
	if (flag == Z_AXIS_ROTATION_KEY) ZAxisRotation(angle);
	else if (flag == X_AXIS_ROTATION_KEY) XAxisRotation(angle);
	else if (flag == Y_AXIS_ROTATION_KEY) YAxisRotation(angle);

	float normVecX = (surfaceDot2.y - surfaceDot1.y) * (surfaceDot3.z - surfaceDot1.z) - (surfaceDot3.y - surfaceDot1.y) * (surfaceDot2.z - surfaceDot1.z);
	float normVecY = (surfaceDot2.z - surfaceDot1.z) * (surfaceDot3.x - surfaceDot1.x) - (surfaceDot3.z - surfaceDot1.z) * (surfaceDot2.x - surfaceDot1.x);
	float normVecZ = (surfaceDot2.x - surfaceDot1.x) * (surfaceDot3.y - surfaceDot1.y) - (surfaceDot3.x - surfaceDot1.x) * (surfaceDot2.y - surfaceDot1.y);

	normVec = vectorNormalize({ normVecX, normVecY, normVecZ }, { 0,0,0 });

	CentreCoordCalc();

	localCentreCoord = surfaceDot1;
	localXVec = localCentreCoord + left;
	localYVec = localCentreCoord + down;

	//localSurfaceDot2 = toLocalTriangleCoords(surfaceDot2);
	//localSurfaceDot3 = toLocalTriangleCoords(surfaceDot3);
}

bool Triangle::isDotInTriangle(COORDS dot, COORDS triangleDot1, COORDS triangleDot2, COORDS triangleDot3) const noexcept
{
	float num1 = (triangleDot1.x - dot.x) * (triangleDot2.y - triangleDot1.y) - (triangleDot2.x - triangleDot1.x) * (triangleDot1.y - dot.y);
	float num2 = (triangleDot2.x - dot.x) * (triangleDot3.y - triangleDot2.y) - (triangleDot3.x - triangleDot2.x) * (triangleDot2.y - dot.y);
	float num3 = (triangleDot3.x - dot.x) * (triangleDot1.y - triangleDot3.y) - (triangleDot1.x - triangleDot3.x) * (triangleDot3.y - dot.y);

	if (IsSign(num1, num2) && IsSign(num1, num3) && IsSign(num2, num3))
		return true;

	return false;
}

void Triangle::GetIntersections(COORDS dot, COORDS triangleDot1, COORDS triangleDot2, COORDS triangleDot3, COORDS& resIntersect1, COORDS& resIntersect2) const noexcept
{
	COORDS intersect1 = { 10000,10000,0 }; doIntersect(triangleDot1, triangleDot2, dot, { dot.x + 1, dot.y, 0 }, intersect1);
	COORDS intersect2 = { 10000,10000,0 }; doIntersect(triangleDot1, triangleDot3, dot, { dot.x + 1, dot.y, 0 }, intersect2);
	COORDS intersect3 = { 10000,10000,0 }; doIntersect(triangleDot2, triangleDot3, dot, { dot.x + 1, dot.y, 0 }, intersect3);

	std::vector<COORDS> resGeneralIntersections;
	if (intersect1.x != 10000 && intersect1.y != 10000 && (isDotInTriangle({ intersect1.x - 0.001f, intersect1.y, 0 }, triangleDot1, triangleDot2, triangleDot3) || isDotInTriangle({ intersect1.x + 0.001f, intersect1.y, 0 }, triangleDot1, triangleDot2, triangleDot3))) resGeneralIntersections.push_back(intersect1);
	if (intersect2.x != 10000 && intersect2.y != 10000 && (isDotInTriangle({ intersect2.x - 0.001f, intersect2.y, 0 }, triangleDot1, triangleDot2, triangleDot3) || isDotInTriangle({ intersect2.x + 0.001f, intersect2.y, 0 }, triangleDot1, triangleDot2, triangleDot3))) resGeneralIntersections.push_back(intersect2);
	if (intersect3.x != 10000 && intersect3.y != 10000 && (isDotInTriangle({ intersect3.x - 0.001f, intersect3.y, 0 }, triangleDot1, triangleDot2, triangleDot3) || isDotInTriangle({ intersect3.x + 0.001f, intersect3.y, 0 }, triangleDot1, triangleDot2, triangleDot3))) resGeneralIntersections.push_back(intersect3);

	// !!!
	if (resGeneralIntersections.size() >= 2)
	{
		resIntersect1 = resGeneralIntersections[0];
		resIntersect2 = resGeneralIntersections[1];
	}
	else
	{
		resIntersect1 = { 0,0,0 };
		resIntersect2 = { 0,0,0 };
	}
}

COORDS Triangle::TriangleDotToLocalTextureCoords(COORDS circleDot) const
{
	circleDot = toLocalTriangleCoords(circleDot);

	float angle = GetAngleBetweenVectors({ 0,0,0 }, circleDot, localSurfaceDot2) * (texturesCalcVars.angleDot > 0 ? 1 : -1);
	circleDot = VecZAxisRotation(vectorNormalize(texturesCalcVars.oldGeneralDot2, { 0,0,0 }), angle) * dotDistance({ 0,0,0 }, circleDot);

	circleDot = circleDot * texturesCalcVars.delta;
	circleDot = circleDot + COORDS{ 0, texturesCalcVars.maxY, 0 };

	COORDS generalIntersection1, generalIntersection2;
	COORDS otherIntersection1, otherIntersection2;
	GetIntersections(circleDot, texturesCalcVars.generalDot1, texturesCalcVars.generalDot2, texturesCalcVars.generalDot3, generalIntersection1, generalIntersection2);
	GetIntersections(circleDot, texturesCalcVars.otherDot1, texturesCalcVars.otherDot2, texturesCalcVars.otherDot3, otherIntersection1, otherIntersection2);

	float del = dotDistance(generalIntersection1, generalIntersection2);
	del = (del == 0 ? 0.01 : del);
	float delta2 = dotDistance(otherIntersection1, otherIntersection2) / del;
	COORDS result = COORDS{ min(otherIntersection1.x, otherIntersection2.x), circleDot.y, 0 } + COORDS{ 1,0,0 } *dotDistance(circleDot, COORDS{ min(generalIntersection1.x, generalIntersection2.x), circleDot.y, 0 }) * delta2;
	return result + textureTriangleDot1;
}

float Triangle::TriangleSurfaceDotCalc(COORDS orig, COORDS dir, float minDotAndCameraDistValue)
{
	COORDS e1 = { surfaceDot2.x - surfaceDot1.x, surfaceDot2.y - surfaceDot1.y, surfaceDot2.z - surfaceDot1.z };
	COORDS e2 = { surfaceDot3.x - surfaceDot1.x, surfaceDot3.y - surfaceDot1.y, surfaceDot3.z - surfaceDot1.z };

	COORDS pvec = { (dir.y * e2.z - dir.z * e2.y), -(dir.x * e2.z - dir.z * e2.x), (dir.x * e2.y - dir.y * e2.x) };
	float det = (e1.x * pvec.x + e1.y * pvec.y + e1.z * pvec.z);

	if (det < 1e-8 && det > -1e-8) {
		return 0;
	}

	float inv_det = 1 / det;
	COORDS tvec = { orig.x - surfaceDot1.x, orig.y - surfaceDot1.y, orig.z - surfaceDot1.z };
	float u = (tvec.x * pvec.x + tvec.y * pvec.y + tvec.z * pvec.z) * inv_det;
	if (u < 0 || u > 1) {
		return 0;
	}

	COORDS qvec = { (tvec.y * e1.z - tvec.z * e1.y), -(tvec.x * e1.z - tvec.z * e1.x), (tvec.x * e1.y - tvec.y * e1.x) };
	float v = (dir.x * qvec.x + dir.y * qvec.y + dir.z * qvec.z) * inv_det;
	if (v < 0 || u + v > 1) {
		return 0;
	}
	return (e2.x * qvec.x + e2.y * qvec.y + e2.z * qvec.z) * inv_det;
}

COORDS Triangle::toLocalTriangleCoords(COORDS dot) const
{
	COORDS result = { 0,0,0 };

	float localXVecAndDotAngle = GetAngleBetweenVectors(localCentreCoord, localXVec, dot);
	if (localXVecAndDotAngle <= 90)
	{
		float x = dotDistance(localCentreCoord, dot) * cos(fromDegreesToRad(localXVecAndDotAngle));
		float y = dotDistance(localCentreCoord, dot) * sin(fromDegreesToRad(localXVecAndDotAngle));

		result = { x, GetAngleBetweenVectors(localCentreCoord, localYVec, dot) <= 90 ? y : -y, 0 };
	}
	else
	{
		float x = dotDistance(localCentreCoord, dot) * cos(fromDegreesToRad(180 - localXVecAndDotAngle));
		float y = dotDistance(localCentreCoord, dot) * sin(fromDegreesToRad(180 - localXVecAndDotAngle));

		result = { -x, GetAngleBetweenVectors(localCentreCoord, localYVec, dot) <= 90 ? y : -y, 0 };
	}

	return result;
}

void Triangle::TextureCalcVarsInit()
{
	texturesCalcVars.generalDot1 = { 0,0,0 };
	texturesCalcVars.generalDot2 = localSurfaceDot2;
	texturesCalcVars.generalDot3 = localSurfaceDot3;

	texturesCalcVars.otherDot1 = { 0,0,0 };
	texturesCalcVars.otherDot2 = textureTriangleDot2 - textureTriangleDot1;
	texturesCalcVars.otherDot3 = textureTriangleDot3 - textureTriangleDot1;

	texturesCalcVars.generalDot2 = vectorNormalize(texturesCalcVars.otherDot2, { 0,0,0 }) * dotDistance({ 0,0,0 }, texturesCalcVars.generalDot2);
	texturesCalcVars.oldGeneralDot2 = texturesCalcVars.generalDot2;
	texturesCalcVars.angleDot = GetAngleBetweenVectors({ 0,0,0 }, localSurfaceDot2, texturesCalcVars.generalDot3);
	texturesCalcVars.angleDot *= (GetAngleBetweenVectors({ 0,0,0 }, VecZAxisRotation(texturesCalcVars.generalDot2, texturesCalcVars.angleDot), texturesCalcVars.otherDot3) < GetAngleBetweenVectors({ 0,0,0 }, VecZAxisRotation(texturesCalcVars.generalDot2, -texturesCalcVars.angleDot), texturesCalcVars.otherDot3) ? 1 : -1);
	texturesCalcVars.generalDot3 = VecZAxisRotation(vectorNormalize(texturesCalcVars.generalDot2, { 0,0,0 }), texturesCalcVars.angleDot) * dotDistance({ 0,0,0 }, texturesCalcVars.generalDot3);

	float del = abs(maxNumb(texturesCalcVars.generalDot1.y, texturesCalcVars.generalDot2.y, texturesCalcVars.generalDot3.y) - minNumb(texturesCalcVars.generalDot1.y, texturesCalcVars.generalDot2.y, texturesCalcVars.generalDot3.y));
	del = (del == 0 ? 0.01 : del);
	texturesCalcVars.delta = abs(maxNumb(texturesCalcVars.otherDot1.y, texturesCalcVars.otherDot2.y, texturesCalcVars.otherDot3.y) - minNumb(texturesCalcVars.otherDot1.y, texturesCalcVars.otherDot2.y, texturesCalcVars.otherDot3.y)) / del;
	texturesCalcVars.generalDot1 = texturesCalcVars.generalDot1 * texturesCalcVars.delta;
	texturesCalcVars.generalDot2 = texturesCalcVars.generalDot2 * texturesCalcVars.delta;
	texturesCalcVars.generalDot3 = texturesCalcVars.generalDot3 * texturesCalcVars.delta;

	texturesCalcVars.maxY = maxNumb(texturesCalcVars.otherDot1.y, texturesCalcVars.otherDot2.y, texturesCalcVars.otherDot3.y) - maxNumb(texturesCalcVars.generalDot1.y, texturesCalcVars.generalDot2.y, texturesCalcVars.generalDot3.y);
	texturesCalcVars.generalDot1 = texturesCalcVars.generalDot1 + COORDS{ 0, texturesCalcVars.maxY, 0 };
	texturesCalcVars.generalDot2 = texturesCalcVars.generalDot2 + COORDS{ 0, texturesCalcVars.maxY, 0 };
	texturesCalcVars.generalDot3 = texturesCalcVars.generalDot3 + COORDS{ 0, texturesCalcVars.maxY, 0 };
}

void Triangle::SetTextureParams(std::string textureName, COORDS textureTriangleDot1, COORDS textureTriangleDot2, COORDS textureTriangleDot3) noexcept
{
	for (size_t i = 0; i < preLoadTextures.size(); ++i)
	{
		if (textureName == preLoadTextures[i].textureName)
		{
			currentTexturesInfo = &preLoadTextures[i];
			break;
		}
	}

	this->textureTriangleDot1 = { textureTriangleDot1.x * currentTexturesInfo->bitmap.wight, textureTriangleDot1.y * currentTexturesInfo->bitmap.height, textureTriangleDot1.z };
	this->textureTriangleDot2 = { textureTriangleDot2.x * currentTexturesInfo->bitmap.wight, textureTriangleDot2.y * currentTexturesInfo->bitmap.height, textureTriangleDot2.z };
	this->textureTriangleDot3 = { textureTriangleDot3.x * currentTexturesInfo->bitmap.wight, textureTriangleDot3.y * currentTexturesInfo->bitmap.height, textureTriangleDot3.z };

	localCentreCoord = surfaceDot1;
	localXVec = localCentreCoord + left;
	localYVec = localCentreCoord + down;

	localSurfaceDot2 = toLocalTriangleCoords(surfaceDot2);
	localSurfaceDot3 = toLocalTriangleCoords(surfaceDot3);

	TextureCalcVarsInit();
}

int Triangle::GetColor() const noexcept
{
	return 0;
}

COORDS Triangle::GetColor(COORDS triangleDot) const
{
	COORDS newCircleDot = TriangleDotToLocalTextureCoords(triangleDot);
	COORDS scalingNewCircleDot = newCircleDot;

	/*if (std::isnan(texturesCalcVars.generalDot2.x) || std::isnan(texturesCalcVars.generalDot2.y)
		|| std::isnan(texturesCalcVars.generalDot3.x) || std::isnan(texturesCalcVars.generalDot3.y))
	{
		mut.lock();
		std::cout << localSurfaceDot2.x << ' ' << localSurfaceDot2.y << '\n';
		std::cout << localSurfaceDot3.x << ' ' << localSurfaceDot3.y << '\n';
	}*/

	int line, column;

	try
	{
		if (scalingNewCircleDot.y > 0)
			// от bitmap.height отнимаем все остальное, так как в obj файле начало системы координат располагается немного по другому (а именное в левом нижнем углу). Следовательно нам нужно перевернуть это изображение. В методе SetTextureParams мы не могли этого сделать так как в значении y могло быть число большее bitmap.height либо меньшее нуля
			line = currentTexturesInfo->bitmap.height - (int)round(scalingNewCircleDot.y) % (int)currentTexturesInfo->bitmap.height;
		else
			line = currentTexturesInfo->bitmap.height - (currentTexturesInfo->bitmap.height - abs((int)round(scalingNewCircleDot.y)) % (int)currentTexturesInfo->bitmap.height);

		if (scalingNewCircleDot.x > 0)
			column = (int)round(scalingNewCircleDot.x) % (int)currentTexturesInfo->bitmap.wight;
		else
			column = currentTexturesInfo->bitmap.wight - abs((int)round(scalingNewCircleDot.x)) % (int)currentTexturesInfo->bitmap.wight;

		currentTexturesInfo->pixelMatrix.at(line).at(column);
	}
	catch (...)
	{
		line = column = 0;
	}

	BmpPixel currentPolygonRGBColor = (currentTexturesInfo->pixelMatrix)[line][column];
	return { (float)currentPolygonRGBColor.red, (float)currentPolygonRGBColor.green, (float)currentPolygonRGBColor.blue };
}

bool Triangle::isObjHasTexture() const noexcept
{
	return true;
}