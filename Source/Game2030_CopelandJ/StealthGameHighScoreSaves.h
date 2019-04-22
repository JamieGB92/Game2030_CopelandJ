// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "StealthGameHighScoreSaves.generated.h"

/**
 * 
 */
UCLASS()
class GAME2030_COPELANDJ_API UStealthGameHighScoreSaves : public USaveGame
{
	GENERATED_BODY()

public :
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlot;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
		float score;

	UStealthGameHighScoreSaves();

};
