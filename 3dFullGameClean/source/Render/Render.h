#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include "GlobalVars.h"
#include "AActor.h"
#include "ACamera.h"
#include "Maths.h"

static const float DRAWING_RANGE = 100000;
static std::string gradNew = "@#W%@#W%*+=,-.";

void equationSystemSolving(float firstIntervalStart, float firstIntervalEnd, float secondIntervalStart, float secondIntervalEnd,
    bool& isSolutionExists, float& minValueInSolution, float& maxValueInSolution, float rayLength = DRAWING_RANGE) noexcept;

bool isLightHitsDot(const std::vector<AActor*>& actors, COORDS nearCrossRayAndFigureCoord, COORDS lightCoord, const objectType& rayCrossedObjType, int rayCrossedObjNumber, int rayHitSurfaceNumber);

void ifIsParallelepiped(const std::vector<AActor*>& actors, Range& tempParRange, const objectType& rayCrossedObjType, int figureNumber, int rayCrossedObjNumber) noexcept;

bool IsLightRayCrossesParallelepiped(const std::vector<AActor*>& actors, COORDS lightCoord, COORDS tempDotCoord, int figureNumber, int rayCrossedObjNumber, int rayHitSurfaceNumber);

void RayCastingFromCamera(const std::vector<AActor*>& actors, const ACamera& playerCamera, COORDS rightUpCamCoord, COORDS cameraVecUpOrDown, COORDS cameraVecLeftOrRight, int yStart, int yEnd, bool isMultiplayer);

void OnePixelHandle(const std::vector<AActor*>& actors, float xScreenPixelNumb, float yScreenPixelNumb, const ACamera& playerCamera, COORDS rightUpCamCoord, COORDS cameraDownVec, COORDS cameraLeftVec, bool isMultiplayer);

COORDS CameraNormalizeRayCoordCalc(int xScreenPixelNumb, int yScreenPixelNumb, COORDS cameraVecUpOrDown, COORDS cameraVecLeftOrRight, COORDS rightUpCamCoord, COORDS verCamDir, COORDS cameraCoord);

COORDS LimpidColorHandle(const std::vector<AActor*>& actors, COORDS cameraCoord, COORDS cameraNormalizeRay, COORDS finalNearCrossRayAndFigureCoord, float rayLenght);

float RayLengthCalc(const std::vector<AActor*>& actors, COORDS cameraCoord, COORDS cameraNormalizeRay);

void RayCrossSystemEquationSolving(const Range& figureRange, COORDS raySourceCoord, COORDS normalizeRay,
    bool& isEquationSolutionExists, float& nearCrossRayAndFigureValue, float& farCrossRayAndFigureValue, float rayLength = DRAWING_RANGE) noexcept;

bool IsRayCrossesParallelepiped(const std::vector<AActor*>& actors, COORDS cameraNormalizeRay, COORDS cameraCoord, int figureNumber,
    float farCrossRayAndFigureValue, COORDS& dotCoord, int& rayHitSurfaceNumber) noexcept;

void Rendering(const std::vector<AActor*>& actors, const ACamera& playerCamera, bool isMultiplayer);

void ShowImage(bool isMultiplayer);

void ClearImage();