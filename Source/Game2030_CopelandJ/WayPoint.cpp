// Fill out your copyright notice in the Description page of Project Settings.

#include "WayPoint.h"
#include "AIGuard.h"

// Sets default values
AWayPoint::AWayPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SetRootComponent(Root);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	BoxComp->SetupAttachment(GetRootComponent());
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AWayPoint::OnPlayerEnter);

}

// Called when the game starts or when spawned
void AWayPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWayPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWayPoint::OnPlayerEnter(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	bool doOnce = false;
	AAIGuard* Character = nullptr;
		if (OtherActor != nullptr)
		{
			Character = Cast<AAIGuard>(OtherActor);
			if (Character != nullptr)
			{
				
				Character->resetPatrol = true;
			}
		}

}

