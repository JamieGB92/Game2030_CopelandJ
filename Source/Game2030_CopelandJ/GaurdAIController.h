// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "GaurdAIController.generated.h"

/**
 * 
 */
UCLASS()
class GAME2030_COPELANDJ_API AGaurdAIController : public AAIController
{
	GENERATED_BODY()


		UPROPERTY(transient)
		class UBlackBoardComponent* BlackBoardComp;

		UPROPERTY(transient)
		class UBehaviorTreeComponent* BehaviorTreeComp;

		

public:
	
	AGaurdAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UAISenseConfig_Sight* SightConfig;

	

#pragma region overides

	virtual void BeginPlay()					override;

	virtual void Tick(float DeltaSeconds)		override;

	virtual FRotator GetControlRotation() const override;

	virtual void Possess(APawn* InPawn)			override;

	//virtual void OnB
#pragma endregion



	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*> &DetectedPawns);

#pragma region Ai Perception variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AiSightRadius = 500.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AiSightAge = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AiLoseSightRadius = AiSightRadius + 50.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AiFOV = 45.f;
#pragma endregion
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool bIsPlayerDetected = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float DistanceToPlayer = 0.0f;

	FVector lookDirection, initLocal;
	FRotator lookRot, initRot;
	FQuat lookQuat;
	float RotRate = 1.f;
	float checkRot = 45.f;
	float initRotYaw;
	bool resetPatrol = false;
	bool doOnce = false;

};
