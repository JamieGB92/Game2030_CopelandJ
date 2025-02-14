// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Game2030_CopelandJCharacter.h"
#include "Engine/Engine.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"

//////////////////////////////////////////////////////////////////////////
// AGame2030_CopelandJCharacter

//class UPawnNoiseEmitterComponent;

AGame2030_CopelandJCharacter::AGame2030_CopelandJCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	PrimaryActorTick.bCanEverTick = true;
	// set our turn rates for input
	BaseTurnRate = 180.f;
	BaseLookUpRate = 500.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	
		
	turnRate = 2;



	
}

void AGame2030_CopelandJCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	


	if (bLevelComplete)
	{
		finalTime = pTime;
	}
	else
	{
		pTime += DeltaTime;
	}
	if (bisDead)
	{
		finalTime = 0;
		GetCharacterMovement()->MaxWalkSpeed = 0;
		turnRate = 0;
		BaseTurnRate = 0.f;
		
	}

	FString levelName = GetWorld()->GetMapName();
	
	
}

void AGame2030_CopelandJCharacter::BeginPlay()
{
	Super::BeginPlay();

	FString mapnameActual = "ThirdPersonExampleMap";
	FString mapName = GetWorld()->GetMapName().Mid(GetWorld()->StreamingLevelsPrefix.Len());
	//GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Red, mapName);
	if (mapName == mapnameActual)
	{

		GetCharacterMovement()->MaxWalkSpeed = 600;
		turnRate = 2;
		BaseTurnRate = 180.f;
		bisDead = false;
		bLevelComplete = false;
		FString poop("scoop");
		GEngine->AddOnScreenDebugMessage(7, 0.f, FColor::Red, poop);
	}

	
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGame2030_CopelandJCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGame2030_CopelandJCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGame2030_CopelandJCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("CameraYaw", this, &AGame2030_CopelandJCharacter::TurnAtRight);

	PlayerInputComponent->BindAction("CameraYawLeft", IE_Pressed, this, &AGame2030_CopelandJCharacter::CameraYawLeft);
	PlayerInputComponent->BindAction("CameraYawRight", IE_Pressed, this, &AGame2030_CopelandJCharacter::CameraYawRight);
	
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGame2030_CopelandJCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGame2030_CopelandJCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGame2030_CopelandJCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AGame2030_CopelandJCharacter::OnResetVR);
}


void AGame2030_CopelandJCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AGame2030_CopelandJCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AGame2030_CopelandJCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AGame2030_CopelandJCharacter::TurnAtRight(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());

}

void AGame2030_CopelandJCharacter::CameraYawLeft()
{
	AddControllerYawInput(GetControlRotation().Yaw-BaseTurnRate);
}

void AGame2030_CopelandJCharacter::CameraYawRight()
{
	AddControllerYawInput(GetControlRotation().Yaw + BaseTurnRate);
	//AddControllerYawInput(GetActorRotation().Yaw + 180.f);
}




void AGame2030_CopelandJCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGame2030_CopelandJCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = GetActorRotation();
		FRotator newRot;
		FRotator YawRotation(0, Rotation.Yaw, 0);
		FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		
	
		
		
		if (Value < 0.f)
		{
			
			bIsWalkingbackwards = true;
			AddMovementInput(Direction, Value);
		}
		else
		{
			bIsWalkingbackwards = false;
			AddMovementInput(Direction, Value);
			
		}
		
		
		
	}
}

void AGame2030_CopelandJCharacter::MoveRight(float Value)
{
	
	FRotator newRotation = GetActorRotation();
	newRotation.Yaw += Value*turnRate;
	SetActorRotation(newRotation);
	
}
