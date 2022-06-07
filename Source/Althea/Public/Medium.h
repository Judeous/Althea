// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupCounterDisplay.h"
#include "GameFramework/Actor.h"
#include "Medium.generated.h"

UCLASS()
class ALTHEA_API AMedium : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMedium();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* CollisionSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* BloomRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APickupCounterDisplay* CounterDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentShards = 0;
	UPROPERTY(EditAnywhere)
	int DesiredShards = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DesiredShardsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBloomsEverything = false;

private:
	UPROPERTY(EditAnywhere)
	int Area = 0;
	AActor* Aplayer = NULL;
	bool bGrantedAbility = false;
	bool bLinkedToCounterDisplay = false;
	bool bIsDisplayingCounter = false;

	UPROPERTY(EditAnywhere)
		float BloomExpansionSpeed = 0.15f;
	UPROPERTY(EditAnywhere)
		int maxBloomRadius = 1000;

	UPROPERTY(EditAnywhere)
		bool bGivesBloom = true;
	UPROPERTY(EditAnywhere)
		bool bGivesDoubleJump = false;
	UPROPERTY(EditAnywhere)
		bool bGivesGlide = false;
};
