// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/Character.h"
#include "Game2030_CopelandJCharacter.generated.h"

class UPawnNoiseEmitterComponent;

UCLASS(config=Game)
class AGame2030_CopelandJCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AGame2030_CopelandJCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Key Pick up")
		bool bHasKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Key Pick up")
		bool bLevelComplete;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Key Pick up")
		bool bisDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
		bool bIsWalkingbackwards=false;


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = timer)
	float finalTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = timer)
		float pTime;

	float turnRate;
protected:
	 
	FVector2D CameraInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
		UPawnNoiseEmitterComponent* NoiseEmitterComponent;

	
	
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRight(float Rate);

	void CameraYawLeft();
	void CameraYawRight();

	
	

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	

};

