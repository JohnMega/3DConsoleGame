
#include <string>
#include "Platform.h"
#include "ACubemapActor.h"

ACubemapActor::ACubemapActor(AActor* observedObj) : observedObj(observedObj)
{
	this->observedObj->AttachCubemap();
}

void ACubemapActor::BeginPlay()
{ }

void ACubemapActor::Tick()
{ }