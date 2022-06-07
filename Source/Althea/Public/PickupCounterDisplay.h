// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupCounterDisplay.generated.h"

UCLASS()
class ALTHEA_API APickupCounterDisplay : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupCounterDisplay();

	APickupCounterDisplay(int toBeCollected);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DisplayCounter(FVector playerLocation, FVector mediumLocation, int currentShards, int totalShards, float DeltaTime);
	void Hide();
	void IdleCounter(float DeltaTime);

public:
	UPROPERTY(EditAnywhere)
		TArray <class AStaticMeshActor*> counterObjects;
	UPROPERTY(EditAnywhere)
		class USphereComponent* Center;

private:
	bool bIsCurrentlyDisplaying = false;

	FVector PlayerPosition;
	float progressToPlayer = 0;

	UPROPERTY(EditAnywhere)
		UMaterial* OffMaterial;
	UPROPERTY(EditAnywhere)
		UMaterial* OnMaterial;

	UPROPERTY(EditAnywhere)
		float RotationSpeed = 1;

	UPROPERTY(EditAnywhere)
		TArray<float> CounterObjectRotationSpeeds;
	UPROPERTY(EditAnywhere)
		TArray<float> CounterObjectHoverSpeeds;

	//UPROPERTY(EditAnywhere)
	//	TArray<float> AuxRotationSpeed = { 0.1f, 0.1f, 0.1f };
	//UPROPERTY(EditAnywhere)
	//	TArray<USphereComponent*> AuxRows;

	UPROPERTY(EditAnywhere)
		FVector inactivePosition;

		float HoverSpeed = 0.2f;
		float CurrentHoverHeight = 0;
		float MaxHoverHeight = 15;
		float MinHoverHeight = -15;
		bool bHoveringUp;
};
