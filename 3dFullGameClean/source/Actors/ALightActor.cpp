
#include "ALightActor.h"
#include "Circle.h"
#include "ACameraActor.h"

extern ACamerasActor camActor;

ALightActor::ALightActor(Light* lightSource, StaticMesh* staticMesh, int currentTextureIndex) 
	: lightSource(lightSource)
	, staticMesh(staticMesh)
	, currentTextureIndex(currentTextureIndex)
{
	lightColors.push_back({ 0,0,0 });
	lightColors.push_back({ 74,82,7 });
	lightColors.push_back({ 82,44,7 });
	lightColors.push_back({ 7,82,19 });
}

COORDS ALightActor::GetLightColor() const noexcept
{
	return lightColors[currentTextureIndex];
}

void ALightActor::BeginPlay()
{ }

void ALightActor::Tick()
{
	if (staticMesh->GetObjType() == objectType::CIRCLE)
	{
		reinterpret_cast<Circle*>(staticMesh)->SetNormVec(camActor.GetCurrentCamera()->GetCameraCoord());
	}
}