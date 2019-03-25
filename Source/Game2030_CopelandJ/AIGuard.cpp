// Fill out your copyright notice in the Description page of Project Settings.

#include "AIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"

// Sets default values

AAIGuard::AAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GuardSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AUDIO"));
	GuardSoundComponent->bAutoActivate = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed /= 2;
	//PawnSensingComp = CreateAbstractDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingCmp"));
	//PawnSensingComp->OnSeePawn.AddDynamic(this, &AAIGuard::OnPawnSeen);
	GetGuard = this;

	
	

}

// Called when the game starts or when spawned
void AAIGuard::BeginPlay()
{
	Super::BeginPlay();
	//PawnSensingComp->OnHearNoise.AddDynamic(this, &AAIGuard::OnNoiseHeard);
	
}

void AAIGuard::OnPawnSeen(APawn * SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Red, false, 10.f);
}

void AAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	
	DrawDebugSphere(GetWorld(), NoiseInstigator->GetActorLocation(), 32.f, 12, FColor::Blue, false, 10.f);
}

// Called every frame
void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}


