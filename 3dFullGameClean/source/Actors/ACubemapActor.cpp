
#include <string>
#include <Windows.h>
#include "ACubemapActor.h"

ACubemapActor::ACubemapActor(AActor* observedObj) : observedObj(observedObj)
{
	this->observedObj->AttachCubemap();
}

void ACubemapActor::BeginPlay()
{ }

void ACubemapActor::Tick()
{ }