
#include <iostream>
#include <string>
#include "ASkyboxPart.h"
#include "ACameraActor.h"
#include "SkyboxPart.h"

extern ACamerasActor camActor;

ASkyboxPart::ASkyboxPart(StaticMesh* staticMesh, int numberOfSkyboxPart)
	: staticMesh(staticMesh)
	, numbOfPart(numberOfSkyboxPart)
{ }

int ASkyboxPart::GetNumberOfSkyboxPart() const noexcept
{
	return numbOfPart;
}

void ASkyboxPart::BeginPlay()
{ }

void ASkyboxPart::Tick()
{
	SkyboxPart* skyboxPart = (SkyboxPart*)staticMesh;

	float x = (numbOfPart == FORWARD_PART ? (skyboxPart->GetParalSize().y + skyboxPart->GetParalSize().z) / (float)4
		: (numbOfPart == BACK_PART ? -(skyboxPart->GetParalSize().y + skyboxPart->GetParalSize().z) / (float)4 : 0));

	float y = (numbOfPart == LEFT_PART ? (skyboxPart->GetParalSize().x + skyboxPart->GetParalSize().z) / (float)4
		: (numbOfPart == RIGHT_PART ? -(skyboxPart->GetParalSize().x + skyboxPart->GetParalSize().z) / (float)4 : 0));

	float z = (numbOfPart == UP_PART ? (skyboxPart->GetParalSize().x + skyboxPart->GetParalSize().y) / (float)4
		: (numbOfPart == DOWN_PART ? -(skyboxPart->GetParalSize().x + skyboxPart->GetParalSize().y) / (float)4 : 0));

	staticMesh->SetCentreCoord() = COORDS{ x, y, z } + camActor.GetCurrentCamera()->GetCameraCoord();
	staticMesh->UpdateRange();
}