// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyBuildingEscapeGameMode.h"
#include "MyBuildingEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyBuildingEscapeGameMode::AMyBuildingEscapeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
