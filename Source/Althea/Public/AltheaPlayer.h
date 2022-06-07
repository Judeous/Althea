// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AltheaPlayer.generated.h"

UCLASS()
class ALTHEA_API AAltheaPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAltheaPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void MoveForward(float AxisValue);

	UFUNCTION(BlueprintCallable)
		void MoveRight(float AxisValue);

	void Jump();
	void HoldJump();
	void ReleaseJump();

	virtual void Landed(const FHitResult& hit) override;

	UFUNCTION(BlueprintCallable)
		void CollectShard();

	UFUNCTION(BlueprintCallable)
		bool GetHasDoubleJump() { return bHasDoubleJump; }
	UFUNCTION(BlueprintCallable)
		bool GetHasGlide() { return bHasGlide; }
	UFUNCTION(BlueprintCallable)
		bool GetHasBloom() { return bHasBloom; }

	UFUNCTION(BlueprintCallable)
		void SetHasDoubleJump(bool value) { bHasDoubleJump = value; }
	UFUNCTION(BlueprintCallable)
		void SetHasGlide(bool value) { bHasGlide = value; }
	UFUNCTION(BlueprintCallable)
		void SetHasBloom(bool value) { bHasBloom = value; }

	UFUNCTION(BlueprintCallable)
		int GetCollectedShards() { return ShardsCollected; }
	UFUNCTION(BlueprintCallable)
		int TakeShards(int value);

private:
	int ShardsCollected = 0;

	APlayerController* Controller;

	float TimeHoldingJump = 0;
	bool bIsHoldingJump = false;
	int CurrentJumpCount = 0;
	int JumpLimit = 2;
	float TimeSinceJump = 900;
	float JumpLimitTime = 0.25f;
	float timeSinceReleaseJump = 0;

	bool bStartedGliding = false;

	bool bHasDoubleJump = true;
	bool bHasGlide = false;
	bool bHasBloom = false;
};
