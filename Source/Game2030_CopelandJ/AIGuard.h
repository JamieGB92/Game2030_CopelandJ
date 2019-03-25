// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "AIGuard.generated.h"



class UPawnSensingComponent;

UCLASS()
class GAME2030_COPELANDJ_API AAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere, Category = "Behavior")
		class UBehaviorTree* GaurdBhavior;

	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool bPatrol;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
		TArray<AActor*> PatrolPoints;

	UFUNCTION()
		void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
		void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
		class AWayPoint* NextWaypoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
		class AWayPoint* Waypoint2;
	// Called to bind functionality to input
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		bool bIsPlayerDetected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		bool resetPatrol = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		AAIGuard* GetGuard;

	FRotator initRot;
};