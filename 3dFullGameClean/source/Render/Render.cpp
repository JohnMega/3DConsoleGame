
#define OOF_IMPL
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdio>

#include "Render.h"
#include "GlobalVars.h"
#include "Preprocessor.h"
#include "Light.h"
#include "Circle.h"
#include "ALightActor.h"
#include "StaticMesh.h"
#include "Pixel.h"
#include "Model.h"
#include "Triangle.h"
#include "ACharacterActor.h"
#include "oof.h"

extern ACamerasActor camActor;

char image[SCREEN_HEIGHT - 2][SCREEN_WIDTH - 2];
COORDS imageColors[SCREEN_HEIGHT - 2][SCREEN_WIDTH - 2];
int imageActorsIndexes[SCREEN_HEIGHT - 2][SCREEN_WIDTH - 2];
oof::screen screen(SCREEN_WIDTH - 2, SCREEN_HEIGHT - 2, 1, 1, ' ');

void equationSystemSolving(float firstIntervalStart, float firstIntervalEnd, float secondIntervalStart, float secondIntervalEnd,
    bool& isSolutionExists, float& minValueInSolution, float& maxValueInSolution, float rayLength) noexcept
{
    if (((firstIntervalStart >= secondIntervalStart && firstIntervalStart <= secondIntervalEnd) || (firstIntervalEnd >= secondIntervalStart && firstIntervalEnd <= secondIntervalEnd)) ||
        ((secondIntervalStart >= firstIntervalStart && secondIntervalStart <= firstIntervalEnd) || (secondIntervalEnd >= firstIntervalStart && secondIntervalEnd <= firstIntervalEnd)))
    {
        if (firstIntervalStart >= secondIntervalStart && firstIntervalStart <= secondIntervalEnd)
        {
            if (((firstIntervalStart) > rayLength && ((firstIntervalEnd < secondIntervalEnd) ? (firstIntervalEnd) : (secondIntervalEnd)) > rayLength) ||
                (firstIntervalStart < 0 && ((firstIntervalEnd < secondIntervalEnd) ? (firstIntervalEnd) : (secondIntervalEnd)) < 0))
            {
                isSolutionExists = false;
                return;
            }

            if (firstIntervalEnd >= secondIntervalStart && firstIntervalEnd <= secondIntervalEnd)
            {
                minValueInSolution = min(firstIntervalStart, firstIntervalEnd);
                maxValueInSolution = max(firstIntervalStart, firstIntervalEnd);
            }
            else
            {
                minValueInSolution = min(firstIntervalStart, secondIntervalEnd);
                maxValueInSolution = max(firstIntervalStart, secondIntervalEnd);
            }

            if (minValueInSolution < 0) minValueInSolution = 0;
            if (maxValueInSolution < 0) maxValueInSolution = 0;
        }
        else if (firstIntervalEnd >= secondIntervalStart && firstIntervalEnd <= secondIntervalEnd)
        {
            if (((firstIntervalEnd) > rayLength && ((firstIntervalStart > secondIntervalStart) ? (firstIntervalStart) : (secondIntervalStart)) > rayLength) ||
                ((firstIntervalEnd) < 0 && ((firstIntervalStart > secondIntervalStart) ? (firstIntervalStart) : (secondIntervalStart)) < 0))
            {
                isSolutionExists = false;
                return;
            }

            if (firstIntervalStart >= secondIntervalStart && firstIntervalStart <= secondIntervalEnd)
            {
                minValueInSolution = min(firstIntervalStart, firstIntervalEnd);
                maxValueInSolution = max(firstIntervalStart, firstIntervalEnd);
            }
            else
            {
                minValueInSolution = min(secondIntervalStart, firstIntervalEnd);
                maxValueInSolution = max(secondIntervalStart, firstIntervalEnd);
            }

            if (minValueInSolution < 0) minValueInSolution = 0;
            if (maxValueInSolution < 0) maxValueInSolution = 0;
        }
        else if (secondIntervalStart >= firstIntervalStart && secondIntervalStart <= firstIntervalEnd)
        {
            if (((secondIntervalStart) > rayLength && ((secondIntervalEnd < firstIntervalEnd) ? (secondIntervalEnd) : (firstIntervalEnd)) > rayLength) ||
                ((secondIntervalStart) < 0 && ((secondIntervalEnd < firstIntervalEnd) ? (secondIntervalEnd) : (firstIntervalEnd)) < 0))
            {
                isSolutionExists = false;
                return;
            }

            if (secondIntervalEnd >= firstIntervalStart && secondIntervalEnd <= firstIntervalEnd)
            {
                minValueInSolution = min(secondIntervalStart, secondIntervalEnd);
                maxValueInSolution = max(secondIntervalStart, secondIntervalEnd);
            }
            else
            {
                minValueInSolution = min(secondIntervalStart, firstIntervalEnd);
                maxValueInSolution = max(secondIntervalStart, firstIntervalEnd);
            }

            if (minValueInSolution < 0) minValueInSolution = 0;
            if (maxValueInSolution < 0) maxValueInSolution = 0;
        }
        else if (secondIntervalEnd >= firstIntervalStart && secondIntervalEnd <= firstIntervalEnd)
        {
            if (((secondIntervalEnd) > rayLength && ((secondIntervalStart > firstIntervalStart) ? (secondIntervalStart) : (firstIntervalStart)) > rayLength) ||
                ((secondIntervalEnd) < 0 && ((secondIntervalStart > firstIntervalStart) ? (secondIntervalStart) : (firstIntervalStart)) < 0))
            {
                isSolutionExists = false;
                return;
            }

            if (secondIntervalStart >= firstIntervalStart && secondIntervalStart <= firstIntervalEnd)
            {
                minValueInSolution = min(secondIntervalStart, secondIntervalEnd);
                maxValueInSolution = max(secondIntervalStart, secondIntervalEnd);
            }
            else
            {
                minValueInSolution = min(firstIntervalStart, secondIntervalEnd);
                maxValueInSolution = max(firstIntervalStart, secondIntervalEnd);
            }

            if (minValueInSolution < 0) minValueInSolution = 0;
            if (maxValueInSolution < 0) maxValueInSolution = 0;
        }

        isSolutionExists = true;
    }
}

bool isLightHitsDot(const std::vector<AActor*>& actors, COORDS nearCrossRayAndFigureCoord, COORDS lightCoord, const objectType& rayCrossedObjType, int rayCrossedObjNumber, int rayHitSurfaceNumber)
{
    bool IsLightRayCrossesParallelepiped(const std::vector<AActor*>&actors, COORDS lightCoord, COORDS nearCrossRayAndFigureCoord, int figureNumber, int rayCrossedObjNumber, int rayHitSurfaceNumber);
    void ifIsParallelepiped(const std::vector<AActor*>&actors, Range & tempParRange, const objectType & objType, int figureNumber, int rayCrossedObjNumber) noexcept;
    void RayCrossSystemEquationSolving(const Range & figureRange, COORDS raySourceCoord, COORDS normalizeRay, bool& isEquationSolutionExists, float& nearCrossRayAndFigureValue, float& farCrossRayAndFigureValue, float rayLength = DRAWING_RANGE) noexcept;

    Range tempFigureRange;
    COORDS lightNormalizeRay = vectorNormalize(nearCrossRayAndFigureCoord, lightCoord);
    float lightRayLength = dotDistance(lightCoord, nearCrossRayAndFigureCoord);
    bool isEquationSolutionExists;
    float nearCrossRayAndFigureValue, farCrossRayAndFigureValue;

    for (size_t index = 1; index < actors.size(); ++index)
    {
        IS_ACTOR_HAS_TYPE(actors[index], Light) continue;

        IS_ACTOR_HAS_TYPE(actors[index], StaticMesh)
        {
            objectType currActorObjType = actors[index]->GetStaticMesh()->GetObjType();
            if (currActorObjType == objectType::CLIP_WALL || currActorObjType == objectType::TRIGGER || currActorObjType == objectType::AREA_PORTAL 
                || currActorObjType == objectType::VOLUME_SKYBOX || currActorObjType == objectType::SKY_CAMERA) continue;

            isEquationSolutionExists = false;
            tempFigureRange = actors[index]->GetStaticMesh()->GetRange();

            ifIsParallelepiped(actors, tempFigureRange, rayCrossedObjType, index, rayCrossedObjNumber);
            RayCrossSystemEquationSolving(tempFigureRange, lightCoord, lightNormalizeRay, isEquationSolutionExists, nearCrossRayAndFigureValue, farCrossRayAndFigureValue, lightRayLength);

            if (isEquationSolutionExists)
            {
                if (IsLightRayCrossesParallelepiped(actors, lightCoord, nearCrossRayAndFigureCoord, index, rayCrossedObjNumber, rayHitSurfaceNumber))
                    return false;
            }
        }
    }

    return true;
}

void ifIsParallelepiped(const std::vector<AActor*>& actors, Range& tempParRange, const objectType& rayCrossedObjType, int figureNumber, int rayCrossedObjNumber) noexcept
{
    static const float ExtendedValue = 0.1F;

    if (rayCrossedObjType == objectType::PARALLELEPIPED)
    {
        if (figureNumber == rayCrossedObjNumber && !actors[figureNumber]->GetStaticMesh()->IsRotate())
        {
            tempParRange.xStart -= ExtendedValue;
            tempParRange.xEnd += ExtendedValue;
            tempParRange.yStart -= ExtendedValue;
            tempParRange.yEnd += ExtendedValue;
            tempParRange.zStart -= ExtendedValue;
            tempParRange.zEnd += ExtendedValue;
        }
    }
}

bool IsLightRayCrossesParallelepiped(const std::vector<AActor*>& actors, COORDS lightCoord, COORDS tempDotCoord, int figureNumber, int rayCrossedObjNumber, int rayHitSurfaceNumber)
{
    if (actors[figureNumber]->GetStaticMesh()->IsParaboloid())
    {
        if (figureNumber != rayCrossedObjNumber)
        {
            if (actors[figureNumber]->GetStaticMesh()->IsRayCrossParaboloid(lightCoord, tempDotCoord))
                return true;
            else
                return false;
        }
    }

    if (!actors[figureNumber]->GetStaticMesh()->IsRotate())
    {
        if (figureNumber == rayCrossedObjNumber)
        {
            if (actors[figureNumber]->GetStaticMesh()->IsRayCrossNotRotatedFigure(lightCoord, tempDotCoord))
                return true;
            else
                return false;
        }
        else
            return true;
    }
    else
    {
        if (figureNumber == rayCrossedObjNumber)
        {
            if (actors[figureNumber]->GetStaticMesh()->IsRotatedFigureShine(lightCoord, tempDotCoord, rayHitSurfaceNumber))
            {
                return true;
            }
            else
                return false;
        }
        else
        {
            if (actors[figureNumber]->GetStaticMesh()->IsRayCrossRotatedFigure(lightCoord, tempDotCoord))
                return true;
            else
                return false;
        }
    }

    return false;
}

COORDS LimpidColorHandle(const std::vector<AActor*>& actors, COORDS cameraCoord, COORDS cameraNormalizeRay, COORDS finalNearCrossRayAndFigureCoord, float rayLenght)
{
    COORDS resultRGBLimpidColor = {0,0,0};

    for (size_t index = 0; index < actors.size(); ++index)
    {
        IS_ACTOR_HAS_TYPE(actors[index], StaticMesh)
        {
            objectType currentActorObjType = actors[index]->GetStaticMesh()->GetObjType();
            if (currentActorObjType == objectType::CIRCLE)
            {
                if (((Circle*)actors[index]->GetStaticMesh())->IsCircleRender() == false) continue;
                if (!actors[index]->isActorHasLight() && ((Circle*)actors[index]->GetStaticMesh())->GetLimpidMode() == false) continue;

                COORDS dotProjection = ((Circle*)actors[index]->GetStaticMesh())->isRayIntersectCircle(cameraCoord, cameraNormalizeRay * rayLenght + cameraCoord);
                if (dotProjection.x == 0 && dotProjection.y == 0 && dotProjection.z == 0) continue;
                if (!(dotProjection.x >= min(cameraCoord.x, (cameraNormalizeRay * rayLenght + cameraCoord).x) && dotProjection.x <= max(cameraCoord.x, (cameraNormalizeRay * rayLenght + cameraCoord).x)
                    && dotProjection.y >= min(cameraCoord.y, (cameraNormalizeRay * rayLenght + cameraCoord).y) && dotProjection.y <= max(cameraCoord.y, (cameraNormalizeRay * rayLenght + cameraCoord).y)
                    && dotProjection.z >= min(cameraCoord.z, (cameraNormalizeRay * rayLenght + cameraCoord).z) && dotProjection.z <= max(cameraCoord.z, (cameraNormalizeRay * rayLenght + cameraCoord).z)))
                    continue;

                COORDS pixelRGBColor;
                if (actors[index]->GetStaticMesh()->isObjHasTexture())
                {
                    pixelRGBColor = actors[index]->GetStaticMesh()->GetColor(dotProjection);
                    if (pixelRGBColor.x == 0 && pixelRGBColor.y == 0 && pixelRGBColor.z == 0) continue;
                }

                if (dotDistance(cameraCoord, dotProjection) < dotDistance(cameraCoord, finalNearCrossRayAndFigureCoord))
                    resultRGBLimpidColor += actors[index]->GetStaticMesh()->GetColor(dotProjection);
            }
        }
    }

    return resultRGBLimpidColor;
}

float RayLengthCalc(const std::vector<AActor*>& actors, COORDS cameraCoord, COORDS cameraNormalizeRay)
{
    float resultRayLenght = DRAWING_RANGE;
    for (size_t index = 0; index < actors.size(); ++index)
    {
        IS_ACTOR_HAS_TYPE(actors[index], StaticMesh)
        {
            if (actors[index]->GetStaticMesh()->GetObjType() == objectType::AREA_PORTAL)
            {
                bool isEquationSolutionExists = false;
                float nearCrossRayAndFigureValue, farCrossRayAndFigureValue;
                RayCrossSystemEquationSolving(actors[index]->GetStaticMesh()->GetRange(), cameraCoord, cameraNormalizeRay, isEquationSolutionExists, nearCrossRayAndFigureValue, farCrossRayAndFigureValue);

                if (isEquationSolutionExists)
                    resultRayLenght = min(resultRayLenght, nearCrossRayAndFigureValue);
            }
        }
    }

    return resultRayLenght;
}

void OnePixelHandle(const std::vector<AActor*>& actors, float xScreenPixelNumb, float yScreenPixelNumb, const ACamera& playerCamera
    , COORDS rightUpCamCoord, COORDS cameraDownVec, COORDS cameraLeftVec, bool isMultiplayer)
{
    COORDS cameraCoord = playerCamera.GetCameraCoordForRender();
    COORDS verCamDir = playerCamera.GetVerCamDir();

    COORDS cameraNormalizeRay, nearCrossRayAndFigureCoord, finalNearCrossRayAndFigureCoord = { 0,0,0 };
    objectType finalObjType = objectType::TRIANGLE;
    float nearCrossRayAndFigureValue = -1, farCrossRayAndFigureValue = -1, finalNearCrossRayAndFigureValue = -1, minDotAndCameraDist;
    bool isEquationSolutionExists;
    int rayCrossedObjNumber = -1, rayCrossedPolygonNumber = -1, finalRayHitSurfaceNumber = -1, rayHitSurfaceNumber = -1;
    float rayLength = DRAWING_RANGE;

    if (!isMultiplayer)
    if (xScreenPixelNumb == 0 || xScreenPixelNumb == 1 || xScreenPixelNumb == 2 || xScreenPixelNumb == 3 || xScreenPixelNumb == 63) return;

    static const float minDotAndCameraDistValue = 10000;
    cameraNormalizeRay = CameraNormalizeRayCoordCalc(xScreenPixelNumb, yScreenPixelNumb, cameraDownVec, cameraLeftVec, rightUpCamCoord, verCamDir, cameraCoord);
    minDotAndCameraDist = minDotAndCameraDistValue;
    rayHitSurfaceNumber = -1;

    // if this pixel already drawn
    Pixel intermediatePixel(cameraNormalizeRay * 10 + cameraCoord); // 10 is a random number
    intermediatePixel.CalcDotCoordsOnScreen(playerCamera);
    if (static_cast<int>(round(intermediatePixel.GetScreenPosX())) > 1 && static_cast<int>(round(intermediatePixel.GetScreenPosX())) < (SCREEN_WIDTH)-1 && static_cast<int>(round(intermediatePixel.GetScreenPosY())) > 1 && static_cast<int>(round(intermediatePixel.GetScreenPosY())) < SCREEN_HEIGHT - 1)
        if (image[static_cast<int>(round(intermediatePixel.GetScreenPosY())) - 1][static_cast<int>(round(intermediatePixel.GetScreenPosX())) - 1] != ' ')
            return;

    rayLength = RayLengthCalc(actors, cameraCoord, cameraNormalizeRay);
    for (size_t index = 0; index < actors.size(); ++index)
    {
        IS_ACTOR_HAS_TYPE(actors[index], StaticMesh)
        {
            objectType currentActorObjType = actors[index]->GetStaticMesh()->GetObjType();
            if (currentActorObjType == objectType::CIRCLE)
            {
                if (((Circle*)actors[index]->GetStaticMesh())->IsCircleRender() == false) continue;
                if (actors[index]->isActorHasLight() || ((Circle*)actors[index]->GetStaticMesh())->GetLimpidMode()) continue;

                COORDS dotProjection = ((Circle*)actors[index]->GetStaticMesh())->isRayIntersectCircle(cameraCoord, cameraNormalizeRay * rayLength + cameraCoord);
                if (dotProjection.x == 0 && dotProjection.y == 0 && dotProjection.z == 0) continue;
                if (!(dotProjection.x >= min(cameraCoord.x, (cameraNormalizeRay * rayLength + cameraCoord).x) && dotProjection.x <= max(cameraCoord.x, (cameraNormalizeRay * rayLength + cameraCoord).x)
                    && dotProjection.y >= min(cameraCoord.y, (cameraNormalizeRay * rayLength + cameraCoord).y) && dotProjection.y <= max(cameraCoord.y, (cameraNormalizeRay * rayLength + cameraCoord).y)
                    && dotProjection.z >= min(cameraCoord.z, (cameraNormalizeRay * rayLength + cameraCoord).z) && dotProjection.z <= max(cameraCoord.z, (cameraNormalizeRay * rayLength + cameraCoord).z)))
                    continue;

                COORDS pixelRGBColor;
                if (actors[index]->GetStaticMesh()->isObjHasTexture())
                {
                    pixelRGBColor = actors[index]->GetStaticMesh()->GetColor(dotProjection);
                    if (pixelRGBColor.x == 0 && pixelRGBColor.y == 0 && pixelRGBColor.z == 0) continue;
                }

                nearCrossRayAndFigureCoord = dotProjection;
                if (dotDistance(cameraCoord, nearCrossRayAndFigureCoord) < minDotAndCameraDist)
                {
                    finalObjType = objectType::CIRCLE;
                    minDotAndCameraDist = dotDistance(cameraCoord, nearCrossRayAndFigureCoord);
                    finalNearCrossRayAndFigureCoord = nearCrossRayAndFigureCoord;
                    rayCrossedObjNumber = index;
                }
            }
            else if (currentActorObjType == objectType::MODEL)
            {
                if (actors[index]->GetStaticMesh()->IsRender() == false) continue;

                isEquationSolutionExists = false;
                float tempStart, tempEnd;
                RayCrossSystemEquationSolving(((Model*)actors[index]->GetStaticMesh())->GetModelRange(), cameraCoord, cameraNormalizeRay, isEquationSolutionExists, tempStart, tempEnd, rayLength);

                Model checkedModel = *((Model*)actors[index]->GetStaticMesh());
                if (isEquationSolutionExists)
                    for (size_t i = 0; i < checkedModel.GetModelPolygons().size() - 1; ++i)
                    {
                        Triangle checkedTriangle = *((Triangle*)checkedModel.GetModelPolygons()[i]);
                        float lenn = checkedTriangle.TriangleSurfaceDotCalc(cameraCoord, cameraNormalizeRay, minDotAndCameraDistValue);
                        if (lenn <= 0) continue;

                        nearCrossRayAndFigureCoord = cameraNormalizeRay * lenn + cameraCoord;
                        if (dotDistance(cameraCoord, nearCrossRayAndFigureCoord) < minDotAndCameraDist)
                        {
                            finalObjType = objectType::TRIANGLE;
                            minDotAndCameraDist = dotDistance(cameraCoord, nearCrossRayAndFigureCoord);
                            finalNearCrossRayAndFigureCoord = nearCrossRayAndFigureCoord;
                            rayCrossedObjNumber = index;
                            rayCrossedPolygonNumber = i;
                        }
                    }
            }
            else if (currentActorObjType == objectType::CLIP_WALL)
            {
            }
            else if (currentActorObjType == objectType::TRIGGER)
            {
            }
            else if (currentActorObjType == objectType::AREA_PORTAL)
            {
            }
            else if (currentActorObjType == objectType::SKY_CAMERA)
            {
            }
            else
            {
                if (actors[index]->GetStaticMesh()->IsRender() == false) continue;

                isEquationSolutionExists = false;
                RayCrossSystemEquationSolving(actors[index]->GetStaticMesh()->GetRange(), cameraCoord, cameraNormalizeRay, isEquationSolutionExists, nearCrossRayAndFigureValue, farCrossRayAndFigureValue, currentActorObjType == objectType::SKYBOX ? DRAWING_RANGE : rayLength);

                if (isEquationSolutionExists)
                {
                    nearCrossRayAndFigureCoord = { cameraNormalizeRay.x * (nearCrossRayAndFigureValue)+cameraCoord.x, cameraNormalizeRay.y * (nearCrossRayAndFigureValue)+cameraCoord.y, cameraNormalizeRay.z * (nearCrossRayAndFigureValue)+cameraCoord.z };
                    if (!IsRayCrossesParallelepiped(actors, cameraNormalizeRay, cameraCoord, index, farCrossRayAndFigureValue, nearCrossRayAndFigureCoord, rayHitSurfaceNumber))
                        continue;

                    if (dotDistance(cameraCoord, nearCrossRayAndFigureCoord) < minDotAndCameraDist)
                    {
                        if (dotDistance(cameraCoord, nearCrossRayAndFigureCoord) == 0) continue;

                        finalObjType = currentActorObjType;
                        minDotAndCameraDist = dotDistance(cameraCoord, nearCrossRayAndFigureCoord);
                        finalNearCrossRayAndFigureCoord = nearCrossRayAndFigureCoord;
                        finalNearCrossRayAndFigureValue = nearCrossRayAndFigureValue;
                        finalRayHitSurfaceNumber = rayHitSurfaceNumber;
                        rayCrossedObjNumber = index;
                    }
                }
            }
        }
    }
    
    if (minDotAndCameraDist != minDotAndCameraDistValue)
    {
        if (finalObjType == objectType::VOLUME_SKYBOX)
        {
            ACamera skyCamera = playerCamera;
            for(size_t i = 0; i < actors.size(); ++i)
                IS_ACTOR_HAS_TYPE(actors[i], StaticMesh)
                if (actors[i]->GetStaticMesh()->GetObjType() == objectType::SKY_CAMERA)
                {
                    skyCamera.SetCameraCoordForRender() = actors[i]->GetStaticMesh()->GetCentreCoord();
                    skyCamera.SetCameraCoord() = actors[i]->GetStaticMesh()->GetCentreCoord();
                    break;
                }

            
            COORDS cameraVecUp = GetVectorRotateZOnAngle(verCamDir, 90);
            COORDS cameraVecDown = GetVectorRotateZOnAngle(verCamDir, 270);
            COORDS cameraVecLeft = { cameraVecUp.y * verCamDir.z - cameraVecUp.z * verCamDir.y, cameraVecUp.z * verCamDir.x - cameraVecUp.x * verCamDir.z, cameraVecUp.x * verCamDir.y - cameraVecUp.y * verCamDir.x };
            COORDS cameraVecRight = { -cameraVecLeft.x, -cameraVecLeft.y, -cameraVecLeft.z };
            COORDS rightUpCamCoord2 = cameraVecRight * (SCREEN_WIDTH)+cameraVecUp * SCREEN_HEIGHT * 2 + skyCamera.GetCameraCoordForRender();

            OnePixelHandle(actors, xScreenPixelNumb, yScreenPixelNumb, skyCamera, rightUpCamCoord2, cameraDownVec, cameraLeftVec, isMultiplayer);
            return;
        }
    }

    if (minDotAndCameraDist != minDotAndCameraDistValue)
    {
        Pixel resultPixel(finalNearCrossRayAndFigureCoord);
        resultPixel.CalcDotCoordsOnScreen(playerCamera);

        // if this pixel already drawn
        if (static_cast<int>(resultPixel.GetScreenPosX()) > 1 && static_cast<int>(resultPixel.GetScreenPosX()) < (SCREEN_WIDTH)-1 && static_cast<int>(resultPixel.GetScreenPosY()) > 1 && static_cast<int>(resultPixel.GetScreenPosY()) < SCREEN_HEIGHT - 1)
        {
            if (image[static_cast<int>(resultPixel.GetScreenPosY()) - 1][static_cast<int>(resultPixel.GetScreenPosX()) - 1] != ' ')
            {
                return;
            }
        }

        float lightPower, finalLightPower = 0;

        // calc light power for pixel dot
        COORDS resultRGBColor = { 0,0,0 };
        if (actors[rayCrossedObjNumber]->isActorHasCubemap())
        {
            for (size_t i = 0; i < actors.size(); ++i)
            {
                IS_ACTOR_HAS_TYPE(actors[i], Light)
                {
                    lightPower = actors[i]->GetLight()->GetLightPower(finalNearCrossRayAndFigureCoord);

                    if (isLightHitsDot(actors, finalNearCrossRayAndFigureCoord, actors[i]->GetLight()->GetCoord(), finalObjType, rayCrossedObjNumber, finalRayHitSurfaceNumber))
                    {
                        finalLightPower += lightPower;

                        if (ceil(lightPower) >= 3)
                            resultRGBColor += ((ALightActor*)actors[i])->GetLightColor();
                    }
                }
            }
        }
        else
            finalLightPower = '@';

        static const int MaxLightPowerValue = 13;
        static const int MinLightPowerValue = 0;

        if (finalLightPower > MaxLightPowerValue) finalLightPower = MaxLightPowerValue;
        else if (finalLightPower < MinLightPowerValue) finalLightPower = MinLightPowerValue;

        static const int BrightestLightStartValue = 9;
        static const int BrightLightStartValue = 5;
        static const int BrightLightExtendColorValue = 8;
        static const int BrightestLightColorValue = 14;

        int resultConsoleColor;
        if (actors[rayCrossedObjNumber]->GetStaticMesh()->GetObjType() == objectType::MODEL)
            resultConsoleColor = actors[rayCrossedObjNumber]->GetStaticMesh()->GetModelPolygons()[rayCrossedPolygonNumber]->GetColor();
        else
            resultConsoleColor = actors[rayCrossedObjNumber]->GetStaticMesh()->GetColor();


        if (actors[rayCrossedObjNumber]->isActorHasCubemap())
            if (finalLightPower > BrightestLightStartValue && finalLightPower <= MaxLightPowerValue)
                resultConsoleColor = BrightestLightColorValue;
            else if (finalLightPower > BrightLightStartValue && finalLightPower <= BrightestLightStartValue && resultConsoleColor <= 7)
                resultConsoleColor += BrightLightExtendColorValue;


        if (actors[rayCrossedObjNumber]->GetStaticMesh()->isObjHasTexture())
        {
            if (finalObjType == objectType::TRIANGLE)
                resultRGBColor += actors[rayCrossedObjNumber]->GetStaticMesh()->GetModelPolygons()[rayCrossedPolygonNumber]->GetColor(finalNearCrossRayAndFigureCoord);
            else
                resultRGBColor += actors[rayCrossedObjNumber]->GetStaticMesh()->GetColor(finalNearCrossRayAndFigureCoord);
        }
        else
            resultRGBColor += ConsoleRBGColors.GeneralRGBConsoleColors[resultConsoleColor - 1];

        // Parallelepiped parab mode
        if (actors[rayCrossedObjNumber]->GetStaticMesh()->GetObjType() == objectType::PARALLELEPIPED)
        {
            if (actors[rayCrossedObjNumber]->GetStaticMesh()->IsDotInCameraParaboloidRadius(finalNearCrossRayAndFigureCoord, cameraCoord, verCamDir, farCrossRayAndFigureValue))
            {
                resultConsoleColor = 15;
                resultRGBColor += ConsoleRBGColors.GeneralRGBConsoleColors[resultConsoleColor - 1];
            }
        }

        if (static_cast<int>(resultPixel.GetScreenPosX()) > 1 && static_cast<int>(resultPixel.GetScreenPosX()) < (SCREEN_WIDTH)-1 && static_cast<int>(resultPixel.GetScreenPosY()) > 1 && static_cast<int>(resultPixel.GetScreenPosY()) < SCREEN_HEIGHT - 1)
        {
            resultRGBColor += LimpidColorHandle(actors, camActor.GetCurrentCamera()->GetCameraCoord(), cameraNormalizeRay, finalNearCrossRayAndFigureCoord, rayLength);
            resultRGBColor = { resultRGBColor.x > 255 ? 255 : resultRGBColor.x, resultRGBColor.y > 255 ? 255 : resultRGBColor.y, resultRGBColor.z > 255 ? 255 : resultRGBColor.z };

            image[static_cast<int>(resultPixel.GetScreenPosY()) - 1][static_cast<int>(resultPixel.GetScreenPosX()) - 1] = ((image[static_cast<int>(resultPixel.GetScreenPosY()) - 1][static_cast<int>(resultPixel.GetScreenPosX()) - 1] == ' ') ? gradNew[(int)(13 - ceil(finalLightPower))] : image[static_cast<int>(resultPixel.GetScreenPosY()) - 1][static_cast<int>(resultPixel.GetScreenPosX()) - 1]);
            imageColors[static_cast<int>(resultPixel.GetScreenPosY()) - 1][static_cast<int>(resultPixel.GetScreenPosX()) - 1] = resultRGBColor;
            imageActorsIndexes[static_cast<int>(resultPixel.GetScreenPosY()) - 1][static_cast<int>(resultPixel.GetScreenPosX()) - 1] = rayCrossedObjNumber;
        }
    }
}

void RayCastingFromCamera(const std::vector<AActor*>& actors, const ACamera& playerCamera, COORDS rightUpCamCoord, COORDS cameraDownVec, COORDS cameraLeftVec, int yStart, int yEnd, bool isMultiplayer)
{   
    //for (int yScreenPixelNumb = ((firstOrSecondPartOfScreen) ? SCREEN_HEIGHT : SCREEN_HEIGHT * 2); yScreenPixelNumb >= ((firstOrSecondPartOfScreen) ? 0 : SCREEN_HEIGHT); --yScreenPixelNumb)
    for (int yScreenPixelNumb = yStart; yScreenPixelNumb >= yEnd; --yScreenPixelNumb)
    {
        if(!isMultiplayer)
        if (yScreenPixelNumb == 24 || yScreenPixelNumb == 28 || yScreenPixelNumb == 34 || yScreenPixelNumb == 37
            || yScreenPixelNumb == 41 || yScreenPixelNumb == 47 || yScreenPixelNumb == 51 || yScreenPixelNumb >= 54) continue;

        for (size_t xScreenPixelNumb = 0; xScreenPixelNumb < (SCREEN_WIDTH / (isMultiplayer ? 2 : 1)); ++xScreenPixelNumb)
        {
            OnePixelHandle(actors, xScreenPixelNumb, yScreenPixelNumb, playerCamera, rightUpCamCoord, cameraDownVec, cameraLeftVec, isMultiplayer);
        }
    }
}

COORDS CameraNormalizeRayCoordCalc(int xScreenPixelNumb, int yScreenPixelNumb, COORDS cameraVecUpOrDown, COORDS cameraVecLeftOrRight, COORDS rightUpCamCoord, COORDS verCamDir, COORDS cameraCoord)
{
    static const int RayDistanceValue = 100;

    COORDS newCameraCoord = rightUpCamCoord + cameraVecUpOrDown * ((SCREEN_HEIGHT * 2) - yScreenPixelNumb);
    COORDS newCameraCoord2 = newCameraCoord + cameraVecLeftOrRight * xScreenPixelNumb;

    return vectorNormalize({ verCamDir.x * RayDistanceValue + newCameraCoord2.x, verCamDir.y * RayDistanceValue + newCameraCoord2.y, verCamDir.z * RayDistanceValue + newCameraCoord2.z }, cameraCoord);
}

void RayCrossSystemEquationSolving(const Range& figureRange, COORDS raySourceCoord, COORDS normalizeRay,
    bool& isEquationSolutionExists, float& nearCrossRayAndFigureValue, float& farCrossRayAndFigureValue, float rayLength) noexcept
{
    float minX = min((figureRange.xStart - raySourceCoord.x) / normalizeRay.x, (figureRange.xEnd - raySourceCoord.x) / normalizeRay.x);
    float maxX = max((figureRange.xStart - raySourceCoord.x) / normalizeRay.x, (figureRange.xEnd - raySourceCoord.x) / normalizeRay.x);
    float minY = min((figureRange.yStart - raySourceCoord.y) / normalizeRay.y, (figureRange.yEnd - raySourceCoord.y) / normalizeRay.y);
    float maxY = max((figureRange.yStart - raySourceCoord.y) / normalizeRay.y, (figureRange.yEnd - raySourceCoord.y) / normalizeRay.y);
    float minZ = min((figureRange.zStart - raySourceCoord.z) / normalizeRay.z, (figureRange.zEnd - raySourceCoord.z) / normalizeRay.z);
    float maxZ = max((figureRange.zStart - raySourceCoord.z) / normalizeRay.z, (figureRange.zEnd - raySourceCoord.z) / normalizeRay.z);

    if (((minX >= minY && minX <= maxY) || (maxX >= minY && maxX <= maxY)) ||
        ((minY >= minX && minY <= maxX) || (maxY >= minX && maxY <= maxX)))
    {
        if ((minX >= minY && minX <= maxY) && maxX < maxY)
            equationSystemSolving(minX, maxX, minZ, maxZ, isEquationSolutionExists, nearCrossRayAndFigureValue, farCrossRayAndFigureValue, rayLength);

        else if ((minX >= minY && minX <= maxY) && maxX >= maxY)
            equationSystemSolving(minX, maxY, minZ, maxZ, isEquationSolutionExists, nearCrossRayAndFigureValue, farCrossRayAndFigureValue, rayLength);

        else if ((maxX >= minY && maxX <= maxY) && minX > minY)
            equationSystemSolving(minX, maxX, minZ, maxZ, isEquationSolutionExists, nearCrossRayAndFigureValue, farCrossRayAndFigureValue, rayLength);

        else if ((maxX >= minY && maxX <= maxY) && minX <= minY)
            equationSystemSolving(minY, maxX, minZ, maxZ, isEquationSolutionExists, nearCrossRayAndFigureValue, farCrossRayAndFigureValue, rayLength);

        else if ((minY >= minX && minY <= maxX) && maxY < maxX)
            equationSystemSolving(minY, maxY, minZ, maxZ, isEquationSolutionExists, nearCrossRayAndFigureValue, farCrossRayAndFigureValue, rayLength);

        else if ((minY >= minX && minY <= maxX) && maxY >= maxX)
            equationSystemSolving(minY, maxX, minZ, maxZ, isEquationSolutionExists, nearCrossRayAndFigureValue, farCrossRayAndFigureValue, rayLength);

        else if ((maxY >= minX && maxY <= maxX) && minY > minX)
            equationSystemSolving(minY, maxY, minZ, maxZ, isEquationSolutionExists, nearCrossRayAndFigureValue, farCrossRayAndFigureValue, rayLength);

        else if ((maxY >= minX && maxY <= maxX) && minY <= minX)
            equationSystemSolving(minX, maxY, minZ, maxZ, isEquationSolutionExists, nearCrossRayAndFigureValue, farCrossRayAndFigureValue, rayLength);
    }
}

bool IsRayCrossesParallelepiped(const std::vector<AActor*>& actors, COORDS cameraNormalizeRay, COORDS cameraCoord, int figureNumber,
    float farCrossRayAndFigureValue, COORDS& dotCoord, int& rayHitSurfaceNumber) noexcept
{
    if (actors[figureNumber]->GetStaticMesh()->IsRotate())
    {
        if (!actors[figureNumber]->GetStaticMesh()->IsDotDisplayAfterFiguresDel(cameraNormalizeRay, cameraCoord, dotCoord, farCrossRayAndFigureValue, rayHitSurfaceNumber))
            return false;
    }

    if (actors[figureNumber]->GetStaticMesh()->IsParaboloid())
    {
        if (!actors[figureNumber]->GetStaticMesh()->ToParab(cameraNormalizeRay, cameraCoord, dotCoord, farCrossRayAndFigureValue))
            return false;
    }

    return true;
}

void Rendering(const std::vector<AActor*>& actors, const ACamera& playerCamera, bool isMultiplayer)
{
    COORDS verCamDir = playerCamera.GetVerCamDir();

    COORDS cameraVecUp = GetVectorRotateZOnAngle(verCamDir, 90);
    COORDS cameraVecDown = GetVectorRotateZOnAngle(verCamDir, 270);
    COORDS cameraVecLeft = { cameraVecUp.y * verCamDir.z - cameraVecUp.z * verCamDir.y, cameraVecUp.z * verCamDir.x - cameraVecUp.x * verCamDir.z, cameraVecUp.x * verCamDir.y - cameraVecUp.y * verCamDir.x };
    COORDS cameraVecRight = { -cameraVecLeft.x, -cameraVecLeft.y, -cameraVecLeft.z };

    COORDS rightUpCamCoord = cameraVecRight * (SCREEN_WIDTH)+cameraVecUp * SCREEN_HEIGHT * 2 + playerCamera.GetCameraCoordForRender();

    /*std::clock_t start;
    double duration;

    start = std::clock();*/

    int coresCount = std::thread::hardware_concurrency();
    std::vector<std::thread> camThreads;
    float step = 1.2 / (float)coresCount;
    for (int i = 0; i < coresCount; ++i)
    {
        std::thread thr(RayCastingFromCamera, actors, playerCamera, rightUpCamCoord, cameraVecDown * 3, cameraVecLeft * (isMultiplayer ? 4 : 2), SCREEN_HEIGHT * (0.8 + (step * (i + 1))), SCREEN_HEIGHT * (0.8 + (step * i)), isMultiplayer);
        camThreads.push_back(std::move(thr));
    }

    for (int i = 0; i < coresCount; ++i)
    {
        camThreads[i].join();
    }

    /*duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

    system("cls");
    std::cout << "printf: " << duration << '\n';*/
}

void ShowImage(bool isMultiplayer)
{
    std::string str;
    DWORD cWritten;

    if(!isMultiplayer)
    for (size_t i = 0; i < SCREEN_HEIGHT - 2; ++i)
    {
        for (size_t j = 0; j < (SCREEN_WIDTH)-2; ++j)
        {
            if (j > 0 && j <= (SCREEN_WIDTH)-2 - 1)
            {
                if (image[i][j + 1] == ' ')
                {
                    if (j != 21 && j != 22 && j != 36 && j != 37) continue;

                    image[i][j + 1] = image[i][j];
                    imageColors[i][j + 1] = imageColors[i][j];
                    imageActorsIndexes[i][j + 1] = imageActorsIndexes[i][j];
                }
            }
        }
    }
    else
    {
        for (size_t i = 0; i < SCREEN_HEIGHT - 2; ++i)
        {
            for (size_t j = 0; j < (SCREEN_WIDTH)-2; ++j)
            {
                if (j > 0 && j <= (SCREEN_WIDTH)-2 - 1)
                {
                    if (image[i][j] == ' ' && image[i][j - 1] != ' ' && image[i][j + 1] != ' ')
                    {
                        image[i][j] = image[i][j - 1];
                        imageColors[i][j] = imageColors[i][j - 1];
                        imageActorsIndexes[i][j] = (i == (SCREEN_HEIGHT - 2) / 2 && j == (SCREEN_WIDTH - 2) / 2 ? imageActorsIndexes[i][j - 1] : -1);
                    }
                    else if (image[i][j] == ' ' && image[i][j - 1] != ' ' && image[i][j + 1] == ' ' && image[i][j + 2] != ' ')
                    {
                        if (j != 22 && j != 37) continue;

                        image[i][j] = image[i][j - 1];
                        imageColors[i][j] = imageColors[i][j - 1];
                        imageActorsIndexes[i][j] = (i == (SCREEN_HEIGHT - 2) / 2 && j == (SCREEN_WIDTH - 2) / 2 ? imageActorsIndexes[i][j - 1] : -1);;

                        image[i][j + 1] = image[i][j - 1];
                        imageColors[i][j + 1] = imageColors[i][j - 1];
                        imageActorsIndexes[i][j + 1] = (i == (SCREEN_HEIGHT - 2) / 2 && j + 1 == (SCREEN_WIDTH - 2) / 2 ? imageActorsIndexes[i][j - 1] : -1);;
                    }
                    else if (image[i][j] == ' ' && image[i][j - 1] != ' ' && image[i][j + 1] == ' ' && image[i][j + 2] == ' ' && image[i][j + 3] != ' ')
                    {
                        if (j != 29) continue;

                        image[i][j] = image[i][j - 1];
                        imageColors[i][j] = imageColors[i][j - 1];
                        imageActorsIndexes[i][j] = (i == (SCREEN_HEIGHT - 2) / 2 && j == (SCREEN_WIDTH - 2) / 2 ? imageActorsIndexes[i][j - 1] : -1);;

                        image[i][j + 1] = image[i][j - 1];
                        imageColors[i][j + 1] = imageColors[i][j - 1];
                        imageActorsIndexes[i][j + 1] = (i == (SCREEN_HEIGHT - 2) / 2 && j + 1 == (SCREEN_WIDTH - 2) / 2 ? imageActorsIndexes[i][j - 1] : -1);;

                        image[i][j + 2] = image[i][j - 1];
                        imageColors[i][j + 2] = imageColors[i][j - 1];
                        imageActorsIndexes[i][j + 2] = (i == (SCREEN_HEIGHT - 2) / 2 && j + 2 == (SCREEN_WIDTH - 2) / 2 ? imageActorsIndexes[i][j - 1] : -1);;
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < SCREEN_HEIGHT - 2; ++i)
        for (size_t j = 0; j < (SCREEN_WIDTH)-2; ++j)
        {
            screen.get_cell(j, i).m_letter = image[i][j];
            screen.get_cell(j, i).m_format.m_fg_color.red = imageColors[i][j].x;
            screen.get_cell(j, i).m_format.m_fg_color.green = imageColors[i][j].y;
            screen.get_cell(j, i).m_format.m_fg_color.blue = imageColors[i][j].z;
        }

    std::cout << screen.get_string() << oof::reset_formatting();

    ClearImage();
}

void ClearImage()
{
    for (size_t i = 0; i < SCREEN_HEIGHT - 2; ++i)
        for (size_t j = 0; j < (SCREEN_WIDTH)-2; ++j)
        {
            image[i][j] = ' ';
            imageActorsIndexes[i][j] = -2;
        }
}