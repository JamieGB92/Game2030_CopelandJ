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
	initRot.Yaw = Character->GetActorRotation().Yaw;
	
}

void AGaurdAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FString rotDbug =  FString::SanitizeFloat(lookRot.Yaw);
	FString intitRotDebug = FString::SanitizeFloat(lookRot.Yaw);
	//GEngine->AddOnScreenDebugMessage(8, 10.f, FColor::Blue, initLocal.ToString());

	//GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Red, intitRotDebug);
	AAIGuard* Character = Cast<AAIGuard>(GetPawn());
	if (!bTurnPaused)
	{
		PatrolTurnTime += (DeltaSeconds * PatrolTurnSpeed);
		PatrolYaw = FMath::Sin(PatrolTurnTime);
		if (FMath::Abs(PatrolYaw) > 0.9f && ((PatrolYaw > 0.0f) == bTurnLeft))
			bTurnPaused = true;

		const float Spread = 90.0f;
		PatrolYaw *= Spread;
	}
	else
	{
		const float TurnDelay = 5.0f;
		TurnDelayTime += DeltaSeconds;
		if (TurnDelayTime >= TurnDelay)
		{
			TurnDelayTime = 0.0f;
			bTurnPaused = false;
			bTurnLeft = !bTurnLeft;
		}
	}
	
	FString tDebug = FString::SanitizeFloat(PatrolYaw);
	GEngine->AddOnScreenDebugMessage(8, 10.f, FColor::Blue, tDebug);

	if (!DetectedEnemy.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(10, 10.f, FColor::Cyan, FString::Printf(TEXT("Player Is Not  Detected")));
		
		Character->GetCharacterMovement()->MaxWalkSpeed = 100.f;
		if (!resetPatrol)
		{
			MoveToActor(Character->NextWaypoint);
		}
		else
		{
			Character->patrolYaw = PatrolYaw;
			lookRot.Yaw = initRot.Yaw + PatrolYaw;
			Character->SetActorRotation(lookRot);
		}
	
	}
	else if (DetectedEnemy.IsValid())
	{
		//player chase
		
		Character->GetCharacterMovement()->MaxWalkSpeed = 400.f;
		AGame2030_CopelandJCharacter*player = Cast<AGame2030_CopelandJCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		MoveToActor(player, 1.f);
		resetPatrol = false;
		doOnce = false;
		FVector TraceStart = Character->GetActorLocation();
		FVector TraceEnd = player->GetActorLocation();
		FHitResult OutHit;
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedEnemy.Get());
		if (DistanceToPlayer > AiSightRadius)
		{
			DetectedEnemy = nullptr;
		}
		else
		{
			GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
			if (OutHit.Time != 1.0f)
				DetectedEnemy = nullptr;
		}
	}
	//Character->bIsPlayerDetected = bIsPlayerDetected;
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
	if (DetectedEnemy.IsValid())
		return;
	

	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		DetectedEnemy = DetectedPawns[i];
		OnPlayerDetectedDelegate.Broadcast();
		break;
	}
	
}

void AGaurdAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
}

void AGaurdAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
	resetPatrol = true;
}
