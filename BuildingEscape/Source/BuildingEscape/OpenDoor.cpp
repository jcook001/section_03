// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) { UE_LOG(LogTemp, Error, TEXT("%s is missing PressurePlate"), *GetOwner()->GetName()); }
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > MassToOpenDoor)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}

	
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() 
{
	float TotalMass = 0.f;

	//Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//iterate through them adding their mass
	for (const auto& Act : OverlappingActors)
	{
		TotalMass += Act->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		
		//UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Act->GetName())
	}

	//UE_LOG(LogTemp, Warning, TEXT("Total mass on plate = %f"), TotalMass);

	return TotalMass;
}

