// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupCounterDisplay.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Medium.h"
#include "..\Public\PickupCounterDisplay.h"

// Sets default values
APickupCounterDisplay::APickupCounterDisplay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Center = CreateDefaultSubobject<USphereComponent>("Center");

	AMedium* parentMedium = Cast<AMedium>(GetParentActor());
}

// Called when the game starts or when spawned
void APickupCounterDisplay::BeginPlay()
{
	Super::BeginPlay();

	inactivePosition = GetActorLocation();

	CounterObjectRotationSpeeds.SetNum(counterObjects.Num());
	CounterObjectHoverSpeeds.SetNum(counterObjects.Num());

	//Give the counterObjects' material the inactive material
	for (int i = 0; i < counterObjects.Num(); i++) {
		UStaticMeshComponent* meshComponent = counterObjects[i]->GetStaticMeshComponent();
		meshComponent->SetMaterial(0, OffMaterial);

		//Slightly randomize the separate rotation speeds of the CounterObjects
		CounterObjectRotationSpeeds[i] = FMath::RandRange(0.5f, 2.f);
		CounterObjectHoverSpeeds[i] = FMath::RandRange(0.9f, 1.1f);
	}
}

// Called every frame
void APickupCounterDisplay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Calculate the location for the Counter to display on the player
	FVector newDisplayLocation = FVector(PlayerPosition.X,
		PlayerPosition.Y,
		PlayerPosition.Z + 125 + ((FMath::Sin(GetGameTimeSinceCreation() * 1.25f)) * 5));

	//Add or subtract from lerp progress
	progressToPlayer += bIsCurrentlyDisplaying ? DeltaTime : -DeltaTime, IdleCounter(DeltaTime);
	//Clamp the progress between 0 and 1
	progressToPlayer = FMath::Clamp(progressToPlayer, 0.f, 1.f);
	//Move to the calculated position
	Center->SetWorldLocation(FMath::Lerp(inactivePosition, newDisplayLocation, progressToPlayer));

	//Rotate the CounterDisplay around the player with a Sine wave determining the direction.
	//The first multiplication is for the length of time going in one direction. Lower number = Longer length of time going in one direction
	//The second multiplication is for the speed of the rotation. Higher number = Faster rotation.
	AddActorWorldRotation({ 0, (FMath::Sin(GetGameTimeSinceCreation() * 0.25) * 0.5f), 0 });

	//Add the random rotation and hover position
	for (int i = 0; i < counterObjects.Num(); i++) {
		//Random Rotation
		counterObjects[i]->AddActorWorldRotation(FRotator(0, CounterObjectRotationSpeeds[i], 0));
		//Random Z position
		//First multiplication is length of time going up or down (reversed?)
		//Second is speed in the direction
		counterObjects[i]->SetActorLocation(FVector(counterObjects[i]->GetActorLocation().X, counterObjects[i]->GetActorLocation().Y,
			Center->GetComponentLocation().Z + 
				(FMath::Sin(GetGameTimeSinceCreation() * CounterObjectHoverSpeeds[i] * 5) * CounterObjectHoverSpeeds[i] * 2)));
	}
}

void APickupCounterDisplay::DisplayCounter(FVector playerLocation, FVector mediumLocation, int currentShards, int totalShards, float DeltaTime)
{
	PlayerPosition = playerLocation;
	bIsCurrentlyDisplaying = true;

	if (progressToPlayer == 1) {
		//Use currentShards and totalShards to light up the correct number of shards
		for (int i = 0; i < totalShards; i++) {
			//Light up the amount of shards held
			if (i < currentShards) {
				UStaticMeshComponent* meshComponent = counterObjects[i]->GetStaticMeshComponent();
				meshComponent->SetMaterial(0, OnMaterial);
			}
			//Make sure the rest are set to off
			else {
				UStaticMeshComponent* meshComponent = counterObjects[i]->GetStaticMeshComponent();
				meshComponent->SetMaterial(0, OffMaterial);
			}
		}
	}

	//If the CounterDisplay has rotated a full 360 degrees
	if (GetActorRotation().Yaw == 360) {
		//Set the rotation back to zero to prevent overflow just in case
		SetActorRotation({ 0, 0, 0 });
	}

	//Center->SetWorldLocation(newDisplayLocation);
}

void APickupCounterDisplay::Hide()
{
	bIsCurrentlyDisplaying = false;
	//Center->SetWorldLocation(inactivePosition);
}

void APickupCounterDisplay::IdleCounter(float DeltaTime)
{
	//Lerp to inactivePosition modified by a sine wave to make it hover
	FVector newDisplayPosition = FVector(inactivePosition.X,
		inactivePosition.Y,
		inactivePosition.Z + ((FMath::Sin(GetGameTimeSinceCreation() * 1.25f)) * 5));

	//Center->SetWorldLocation(FMath::Lerp(GetActorLocation(), newDisplayPosition, 0.25f));
}