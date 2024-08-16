
#include "AEnvFade.h"
#include "Model.h"
#include "Circle.h"
#include "ACameraActor.h"

extern ACamerasActor camActor;
extern COORDS imageColors[SCREEN_HEIGHT - 2][SCREEN_WIDTH - 2];
extern double tick;

AEnvFade::AEnvFade() 
    : rgbVecLenRatio(SCREEN_HEIGHT - 2, std::vector<COORDS>(SCREEN_WIDTH - 2, {NULL, NULL, NULL}))
    , isOnEnvFadeOff(false)
    , isOnEnvFadeOn(false)
    , fadeOnDelaySum(0.0f)
{ }

void AEnvFade::BeginPlay()
{ }

void AEnvFade::Tick()
{ }

void AEnvFade::ImageTick()
{
    startRgbVecLen = 100 * tick;

    if (isOnEnvFadeOn)
    {
        if (fadeOnDelaySum < fadeOnDelay)
        {
            fadeOnDelaySum += tick;
            for (size_t i = 0; i < SCREEN_HEIGHT - 2; ++i)
                for (size_t j = 0; j < SCREEN_WIDTH - 2; ++j)
                    imageColors[i][j] = { 0,0,0 };

            return;
        }
    }

    for (size_t i = 0; i < SCREEN_HEIGHT - 2; ++i)
        for (size_t j = 0; j < SCREEN_WIDTH - 2; ++j)
        {
            float xCount = imageColors[i][j].x / startRgbVecLen;
            float yLen = imageColors[i][j].y / xCount;
            float zLen = imageColors[i][j].z / xCount;
            COORDS oldImageColor = imageColors[i][j];

            if (oldImageColor.x == 0 && oldImageColor.y == 0 && oldImageColor.z == 0) continue;

            if (isOnEnvFadeOff)
            {
                if (rgbVecLenRatio[i][j].x <= 0 || rgbVecLenRatio[i][j].y <= 0 || rgbVecLenRatio[i][j].z <= 0)
                {
                    imageColors[i][j] = { 0,0,0 };
                    continue;
                }

                imageColors[i][j] = { imageColors[i][j].x * rgbVecLenRatio[i][j].x - startRgbVecLen, imageColors[i][j].y * rgbVecLenRatio[i][j].y - yLen
                    , imageColors[i][j].z * rgbVecLenRatio[i][j].z - zLen };

                if (imageColors[i][j].x <= 0 || imageColors[i][j].y <= 0 || imageColors[i][j].z <= 0)
                {
                    imageColors[i][j] = { 0,0,0 };
                }

                rgbVecLenRatio[i][j] = imageColors[i][j] / oldImageColor;
            }
            else if (isOnEnvFadeOn)
            {
                if (rgbVecLenRatio[i][j].x >= 1 || rgbVecLenRatio[i][j].y >= 1 || rgbVecLenRatio[i][j].z >= 1)
                {
                    continue;
                }

                imageColors[i][j] = { imageColors[i][j].x * rgbVecLenRatio[i][j].x + startRgbVecLen, imageColors[i][j].y * rgbVecLenRatio[i][j].y + yLen
                    , imageColors[i][j].z * rgbVecLenRatio[i][j].z + zLen };

                if (imageColors[i][j].x > 255 || imageColors[i][j].y > 255 || imageColors[i][j].z > 255)
                {
                    imageColors[i][j] = oldImageColor;
                }

                rgbVecLenRatio[i][j] = imageColors[i][j] / oldImageColor;
            }
        }
}