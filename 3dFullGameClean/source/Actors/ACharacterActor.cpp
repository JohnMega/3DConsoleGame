
#include <thread>
#include <iostream>
#include "Windows.h"
#include "ACharacterActor.h"

extern char key;
extern std::vector<class AActor*> actors;
extern POINT NewMouseCoords;
extern POINT OldMouseCoords;

void DefaultCharacterState::CharacterStateKeysHandle(float EndPosition, float* Position, float* Rate, ACamerasActor* camActor
    , COORDS& finalDownDotGravitySum, DefaultCharacterState*& characterState)
{
    if (key == JUMP_KEY) characterState->OnJumpKey(finalDownDotGravitySum, characterState);
    else characterState->OnNothingKey(finalDownDotGravitySum, EndPosition, Position, Rate, camActor, characterState);
}

void DefaultCharacterState::AnimationStateHandle(FantomModel* characterModel)
{
    if (GetAsyncKeyState(VK_FORWARD_KEY) & 0x8000) characterModel->SetCurrentAnimPartIndex(characterModel->RUN_ANIM_MODEL_INDEX);
    else if (GetAsyncKeyState(VK_BACK_KEY) & 0x8000) characterModel->SetCurrentAnimPartIndex(characterModel->BACK_ANIM_MODEL_INDEX);
    else if (GetAsyncKeyState(VK_RIGHT_KEY) & 0x8000) characterModel->SetCurrentAnimPartIndex(characterModel->RIGHT_ANIM_MODEL_INDEX);
    else if (GetAsyncKeyState(VK_LEFT_KEY) & 0x8000) characterModel->SetCurrentAnimPartIndex(characterModel->LEFT_ANIM_MODEL_INDEX);
    else characterModel->SetCurrentAnimPartIndex(characterModel->IDLE_ANIM_MODEL_INDEX);
}

void DefaultCharacterState::OnJumpKey(COORDS& finalDownDotGravitySum, DefaultCharacterState*& characterState)
{
    if (finalDownDotGravitySum.z != 0)
    {
        characterState = CharacterStates::jumpCharacterState;
        finalDownDotGravitySum = { 0,0,0 };
    }
}

void JumpCharacterState::AnimationStateHandle(FantomModel* characterModel)
{
    characterModel->SetCurrentAnimPartIndex(characterModel->JUMP_ANIM_MODEL_INDEX);
}

void JumpCharacterState::OnNothingKey(COORDS finalDownDotGravitySum, float EndPosition, float* Position, float* Rate, ACamerasActor* camActor, DefaultCharacterState*& characterState)
{
    float lastPosition = *Position;
    MotionCubic(EndPosition, (*Position > EndPosition / 2 ? tick * 2.5f : tick), Position, Rate);

    if (*Position >= EndPosition - 0.1)
    {
        characterState = CharacterStates::defaultCharacterState;
        *Position = 0;
        *Rate = 0;
        return;
    }

    camActor->GetCurrentCamera()->SetCameraCoord().z += (*Position > EndPosition / 2 ? -(*Position - lastPosition) : *Position - lastPosition);

    if (finalDownDotGravitySum.z != 0)
    {
        characterState = CharacterStates::defaultCharacterState;
        *Position = 0;
        *Rate = 0;
    }
}

ACharacterActor::ACharacterActor(StaticMesh* staticMesh, ACamerasActor* camActor)
    : staticMesh(staticMesh)
    , characterModel((FantomModel*)staticMesh)
    , camActor(camActor)
    , gravityVector({ 0, 0, -0.1 })
    , characterState(CharacterStates::defaultCharacterState)
    , position(0.0)
    , rate(0.0)
    , desiredPosition(4)
{ }

StaticMesh* ACharacterActor::GetCharacterModel()
{
    return staticMesh;
}

float& ACharacterActor::GetCurrentRotateAngle() noexcept
{
    return currentRotateAngle;
}

void ACharacterActor::BeginPlay()
{ }

void ACharacterActor::CharacterCollisionHandle()
{
    finalCamSum = { 0,0,0 };
    finalDownDotGravitySum = { 0, 0, 0 };

    for (size_t i = 0; i < actors.size(); ++i)
    {
        IS_ACTOR_HAS_TYPE(actors[i], Collision)
        {
            COORDS finalVector = actors[i]->GetCollision()->CollisionHandle(camActor->GetCurrentCamera()->GetLastMoveVector(), camActor->GetCurrentCamera()->GetCameraCoord());
            camActor->GetCurrentCamera()->SetCameraCoord() += (isnan(finalVector.x) || isnan(finalVector.y) || isnan(finalVector.z) ? COORDS{ 0,0,0 } : finalVector);
            finalCamSum += (isnan(finalVector.x) || isnan(finalVector.y) || isnan(finalVector.z) ? COORDS{ 0,0,0 } : finalVector);
        }
    }

    for (size_t i = 0; i < actors.size(); ++i)
    {
        IS_ACTOR_HAS_TYPE(actors[i], Collision)
        {
            COORDS finalVector = actors[i]->GetCollision()->CollisionHandle(gravityVector, (camActor->GetCurrentCamera()->GetCameraCoord()) - COORDS{ 0, 0, (staticMesh->GetRange().zEnd - staticMesh->GetRange().zStart) / 2 });
            camActor->GetCurrentCamera()->SetCameraCoord() += (isnan(finalVector.x) || isnan(finalVector.y) || isnan(finalVector.z) ? COORDS{ 0,0,0 } : finalVector);
            finalDownDotGravitySum += (isnan(finalVector.x) || isnan(finalVector.y) || isnan(finalVector.z) ? COORDS{ 0,0,0 } : finalVector);
        }
    }
}

void ACharacterActor::Tick()
{
    characterState->AnimationStateHandle(characterModel);
    DefaultCharacterState::CharacterStateKeysHandle(desiredPosition, &position, &rate, camActor, finalDownDotGravitySum, characterState);
    characterModel->AnimationHandle();

    if(characterState != CharacterStates::jumpCharacterState)
        camActor->GetCurrentCamera()->SetCameraCoord() += gravityVector * 100 * tick;
    CharacterCollisionHandle();

    characterModel->Rotation(camActor->GetCurrentCamera()->GetHorCamDir());

    OldMouseCoords = NewMouseCoords;
    staticMesh->SetCentreCoord() = camActor->GetCurrentCamera()->GetCameraCoord();
    staticMesh->UpdateRange();
}