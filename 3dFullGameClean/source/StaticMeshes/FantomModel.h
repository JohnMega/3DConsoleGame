#pragma once

#include <vector>
#include <string>
#include "StaticMesh.h"

struct FantomAnimationInfo
{
	COORDS animationDir;
	float tickSum;
	int frameCounter;
	int currentFrame;
	bool isAnimated;
};

using FantomAnimationPart = std::vector<std::vector<StaticMesh*>>;

class FantomModel : public StaticMesh
{
	static inline float nextAnimFrameDelay = 0.05f;

private:
	std::vector<FantomAnimationPart> polygons;
	std::vector<FantomAnimationInfo> animInfo;
	int currAnimPartIndex;

	std::string fileName;
	Range modelRange;
	COORDS currentCentreCoord;
	COORDS oldCentreCoord;

private:
	void CentreCoordCalc();

public:
	static inline int BACK_ANIM_MODEL_INDEX = 0;
	static inline int RUN_ANIM_MODEL_INDEX = 1;
	static inline int RIGHT_ANIM_MODEL_INDEX = 2;
	static inline int LEFT_ANIM_MODEL_INDEX = 3;
	static inline int JUMP_ANIM_MODEL_INDEX = 4;
	static inline int IDLE_ANIM_MODEL_INDEX = 5;

public:
	FantomModel();

	FantomModel(const FantomModel&);

	std::vector<StaticMesh*>& GetModelPolygons() noexcept override;

	std::vector<std::vector<StaticMesh*>>& GetAnimModelPolygons() noexcept;

	void AddNewAnimationPart(const std::string& fileName);

	void Rotation(float angle, size_t flag = 0) override;

	void Rotation(COORDS camDir);

	void AddNewModelSlot();

	int GetCurrentAnimPartIndex();

	void SetCurrentAnimPartIndex(int newCurrAnimPartIndex);

	Range GetRange() const noexcept override;

	COORDS GetCentreCoord() const noexcept override;

	COORDS& SetCentreCoord() noexcept override;

	std::string& SetFileName() noexcept;

	void AnimationHandle() noexcept;

	void UpdateRange(size_t flag = 0) noexcept override;

	FantomAnimationInfo GetAnimationInfo() const noexcept;

	FantomAnimationInfo& SetAnimationInfo() noexcept;

	COORDS GetOldCentreCoord() const noexcept;
};