
#include "Preprocessor.h"
#include "AActor.h"
#include "ACameraActor.h"
#include "Maths.h"
#include "FantomModel.h"

extern double tick;

class DefaultCharacterState
{
public:
    virtual void AnimationStateHandle(FantomModel* characterModel);

    static void CharacterStateKeysHandle(float EndPosition, float* Position, float* Rate, ACamerasActor* camActor, COORDS& finalDownDotGravitySum, DefaultCharacterState*& characterState);

    virtual void OnJumpKey(COORDS& finalDownDotGravitySum, DefaultCharacterState*& characterState);

    virtual void OnNothingKey(COORDS finalDownDotGravitySum, float EndPosition, float* Position, float* Rate, ACamerasActor* camActor, DefaultCharacterState*& characterState)
    { }
};

class JumpCharacterState : public DefaultCharacterState
{
public:
    void AnimationStateHandle(FantomModel* characterModel) override;

    void OnNothingKey(COORDS finalDownDotGravitySum, float EndPosition, float* Position, float* Rate, ACamerasActor* camActor, DefaultCharacterState*& characterState) override;
};

struct CharacterStates
{
    static inline DefaultCharacterState* defaultCharacterState = new DefaultCharacterState;
    static inline JumpCharacterState* jumpCharacterState = new JumpCharacterState;
};

class ACharacterActor : public AActor
{
private:
    ACamerasActor* camActor;
    float currentRotateAngle = 0;
    COORDS gravityVector;
    DefaultCharacterState* characterState;
    FantomModel* characterModel;

    // Jump state vars
    float position;
    float rate;
    float desiredPosition;

    // Collision vars
    COORDS finalCamSum;
    COORDS finalDownDotGravitySum;

    //UPROPERTY(StaticMesh, staticMesh)
    StaticMesh* staticMesh;

private:
    void CharacterCollisionHandle();

public:
    ACharacterActor(StaticMesh* staticMesh, ACamerasActor* camActor);

    StaticMesh* GetCharacterModel();

    float& GetCurrentRotateAngle() noexcept override;

    void BeginPlay() override;

    void Tick() override;
};