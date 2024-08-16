#pragma once

#include "GlobalVars.h"
#include "Maths.h"
#include "StaticMesh.h"
#include "Preprocessor.h"
#include "Model.h"
#include "bitmap.h"

class Triangle : public StaticMesh
{
	struct TexturesInfo
	{
		Bitmap bitmap;
		PixelMatrix pixelMatrix;
		std::string textureName;
	};

	struct TexturesCalcVars
	{
		COORDS generalDot1, generalDot2, generalDot3;
		COORDS oldGeneralDot2;
		COORDS otherDot1, otherDot2, otherDot3;
		float angleDot;
		float delta;
		float maxY;
	};

private:
	Model* ownerModel;
	COORDS surfaceDot1, surfaceDot2, surfaceDot3;
	COORDS normVec;
	COORDS centreCoord;
	COORDS rgbColor;

	// Texture vars
	static inline std::vector<TexturesInfo> preLoadTextures;
	TexturesInfo* currentTexturesInfo;
	TexturesCalcVars texturesCalcVars;
	COORDS localCentreCoord;
	COORDS textureTriangleDot1, textureTriangleDot2, textureTriangleDot3;
	COORDS localSurfaceDot2, localSurfaceDot3;

	COORDS up, down, left, right;

	COORDS localXVec;
	COORDS localYVec;

private:
	void ZAxisRotation(float angle);

	void XAxisRotation(float angle);

	void YAxisRotation(float angle);

	void CentreCoordCalc();

	COORDS TriangleDotToLocalTextureCoords(COORDS circleDot) const;

	COORDS toLocalTriangleCoords(COORDS) const;

	bool isDotInTriangle(COORDS dot, COORDS triangleDot1, COORDS triangleDot2, COORDS triangleDot3) const noexcept;

	void GetIntersections(COORDS dot, COORDS triangleDot1, COORDS triangleDot2, COORDS triangleDot3, COORDS& resIntersect1, COORDS& resIntersect2) const noexcept;

	void TextureCalcVarsInit();

public:
	Triangle(COORDS dot1, COORDS dot2, COORDS dot3, Model* ownerModel, COORDS rgbColor);

	static void PreLoadTextures(std::vector<std::string> preLoadInfo);

	COORDS GetTriangleNormVec() override;

	COORDS GetCentreCoord() const noexcept override;

	COORDS GetSurfaceDot1() const noexcept;
	COORDS GetSurfaceDot2() const noexcept;
	COORDS GetSurfaceDot3() const noexcept;

	void UpdateRange(size_t flag = 0) noexcept override;

	void Rotation(float angle, size_t flag = 0) override;

	void SetNewSurfaceDots(COORDS newSurfaceDot1, COORDS newSurfaceDot2, COORDS newSurfaceDot3);

	float TriangleSurfaceDotCalc(COORDS orig, COORDS dir, float minDotAndCameraDistValue) override;

	int GetColor() const noexcept override;

	COORDS GetColor(COORDS) const override;

	void SetTextureParams(std::string textureName, COORDS x, COORDS y, COORDS z) noexcept;

	bool isObjHasTexture() const noexcept override;
};