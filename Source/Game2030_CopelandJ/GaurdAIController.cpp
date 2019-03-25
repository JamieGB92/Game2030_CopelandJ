// Fill out your copyright notice in the Description page of Project Settings.

#include "GaurdAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Runtime/Engine/Classes/Kismet//GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Game2030_CopelandJCharacter.h"
#include "AIGuard.h"
#include "Engine/Engine.h"
#include "WayPoint.h"
#include "Perception/AISenseConfig_Sight.h"

AGaurdAIController::AGaurdAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	//set up Ai perception component
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Preception Component")));
	//set Ai config variables
	SightConfig->SightRadius = AiSightRadius;
	SightConfig->LoseSightRadius = AiLoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AiFOV;
	SightConfig->SetMaxAge(AiSightAge);
	SightConfig->DetectionByAffiliation.bDetectEnemies		=true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies	=true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals		=true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AGaurdAIController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	lookQuat=FQuat(lookRot);
	//Character = Cast<AAIGuard>(GetPawn());

}

void AGaurdAIController::BeginPlay()
{
	Super::BeginPlay();

	AAIGuard* Character = Cast<AAIGuard>(GetPawn());
	
	initRotYaw = Character->GetActorRotation().Yaw;
	

	initLocal = Character->GetActorLocation();
	
}

void AGaurdAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FString rotDbug =  FString::SanitizeFloat(lookRot.Yaw);
	FString intitRotDebug = FString::SanitizeFloat(lookRot.Yaw);
	//GEngine->AddOnScreenDebugMessage(8, 10.f, FColor::Blue, initLocal.ToString());

	GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Red, intitRotDebug);
	AAIGuard* Character = Cast<AAIGuard>(GetPawn());
	lookRot = Character->GetActorRotation();
	
	
	if (DistanceToPlayer> AiSightRadius)
	{
		bIsPlayerDetected = false;
	}
	if (!bIsPlayerDetected)
	{
		GEngine->AddOnScreenDebugMessage(10, 10.f, FColor::Cyan, FString::Printf(TEXT("Player Is Not  Detected")));
		
		if (!Character->resetPatrol)
		{
			MoveToActor(Character->NextWaypoint);
			GEngine->AddOnScreenDebugMessage(5, 10.f, FColor::Emerald, FString::Printf(TEXT("reset patrol false")));
		}
		else if(Character->resetPatrol)
		{
			
			if (Character->GetActorRotation().Yaw != initRot.Yaw&&!doOnce)
			{
				// reset to initial posisition and rotation 
				Character->SetActorRotation(initRot);//this does not exicute
				doOnce = true;
			}
			//look functionality, oscilliation rotation 
			if (lookRot.Yaw <= initRot.Yaw - 45.f || lookRot.Yaw >= initRot.Yaw + 45.f)
			{
				RotRate *= -1;
			}
			lookRot.Yaw += RotRate;
			Character->SetActorRotation(lookRot);
		}
		
	}
	else if (bIsPlayerDetected)
	{
		//player chase
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Emerald, FString::Printf(TEXT("Player Detected")));
		AGame2030_CopelandJCharacter*player = Cast<AGame2030_CopelandJCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		MoveToActor(player, 1.f);
		Character->resetPatrol = false;
		doOnce = false;
	}
	Character->bIsPlayerDetected = bIsPlayerDetected;
}

FRotator AGaurdAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f,0.0f,0.0f);
	}

	return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.f);
}

void AGaurdAIController::OnPawnDetected(const TArray<AActor*> &DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);
	}
	bIsPlayerDetected = true;
}

void AGaurdAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
}
