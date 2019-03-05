// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Game2030_CopelandJGameMode.h"
#include "Game2030_CopelandJCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGame2030_CopelandJGameMode::AGame2030_CopelandJGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
