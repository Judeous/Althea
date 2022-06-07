// Fill out your copyright notice in the Description page of Project Settings.

#include "AltheaPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAltheaPlayer::AAltheaPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Makes the player rotate towards the direction it's moving, and changes the rate of the turning to be smoother
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

// Called when the game starts or when spawned
void AAltheaPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->AirControl = 0.8f;
}

// Called every frame
void AAltheaPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeSinceJump += DeltaTime;

	if (bIsHoldingJump) {
		TimeHoldingJump += DeltaTime;
		HoldJump();
	}
	else {
		timeSinceReleaseJump += DeltaTime;
		if (timeSinceReleaseJump < 0.25f) {
			GetCharacterMovement()->GravityScale = 1.5f;
			//Slightly lower the horizontal velocity
			GetCharacterMovement()->Velocity.X *= .99f;
			GetCharacterMovement()->Velocity.Y *= .99f;
		}
		else {
			GetCharacterMovement()->GravityScale = 0.8f;
		}
	}
}

// Called to bind functionality to input
void AAltheaPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Reads player movement
	InputComponent->BindAxis("Forward", this, &AAltheaPlayer::MoveForward);
	InputComponent->BindAxis("Right", this, &AAltheaPlayer::MoveRight);

	//Reads camera movement
	InputComponent->BindAxis("Look Up", this, &AAltheaPlayer::AddControllerPitchInput);
	InputComponent->BindAxis("Look Right", this, &AAltheaPlayer::AddControllerYawInput);

	//Reads jump input
	InputComponent->BindAction("Jump", IE_Pressed, this, &AAltheaPlayer::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AAltheaPlayer::ReleaseJump);
}

void AAltheaPlayer::MoveForward(float AxisValue)
{
	if (AxisValue != 0.0f) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void AAltheaPlayer::MoveRight(float AxisValue)
{
	if (AxisValue != 0) {
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, AxisValue);
	}
}
void AAltheaPlayer::Jump()
{
	bIsHoldingJump = true;

	//if the player has not jumped
	if (CurrentJumpCount < 1) {
		//If the player's trying to move in the opposite direction of the current velocity
		if (GetCharacterMovement()->GetCurrentAcceleration().X <= GetCharacterMovement()->Velocity.X * -1) {
			//Lower the horizontal velocity to make swapping to a different direction easier
			GetCharacterMovement()->Velocity.X /= 3.5f;
			GetCharacterMovement()->Velocity.Y /= 3.5f;
		}
		LaunchCharacter({ 0, 0, 600 }, false, true);
		TimeSinceJump = 0;
		CurrentJumpCount++;
	}
	//if the player has already jumped once, check for Double Jump
	else if (CurrentJumpCount > 0 && bHasDoubleJump) {
		//If the player hasn't used all the jumps
		if (CurrentJumpCount < JumpLimit) {
			//If the player's trying to move in the opposite direction of the current velocity
			if (GetCharacterMovement()->GetCurrentAcceleration().X <= GetCharacterMovement()->Velocity.X * -1) {
				//Lower the horizontal velocity to make swapping to a different direction easier
				GetCharacterMovement()->Velocity.X /= 3.5f;
				GetCharacterMovement()->Velocity.Y /= 3.5f;
			}
			LaunchCharacter({ 0, 0, 600 }, false, true);
			TimeSinceJump = 0;
			CurrentJumpCount++;
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString("Jump Pressed"));
}

void AAltheaPlayer::HoldJump()
{
	//If jump has been being held for less than .75 seconds
	if (TimeHoldingJump < .75f) {
		//Add a bit more height to the jump by lowering gravity
		GetCharacterMovement()->GravityScale = 0.4;
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString("Jumping"));
	}
	//Otherwise, and if the player has Glide
	else if (bHasGlide == true) {
		//If the player has jumped all the jumps or if Jump has been being held for more than .75 seconds
		if (CurrentJumpCount >= JumpLimit || TimeHoldingJump >= .75f) {
			//Slightly raise gravity to stop floating up
			GetCharacterMovement()->GravityScale = 0.5;

			//If the player just started gliding
			if (!bStartedGliding) {
				//Half the Z velocity
				GetCharacterMovement()->Velocity.Z /= 2;
				bStartedGliding = true;
			}
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString("Gliding"));

			//If the downwards velocity is too large
			if (GetCharacterMovement()->Velocity.Z <= -150)
			{
				//Lower the downwards velocity
				GetCharacterMovement()->Velocity.Z = -150;
				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString("Lowering Velocity"));
			}
		}
	}
	//If the player has been holding Jump for too long
	else if (TimeHoldingJump > 1.2f) {
		GetCharacterMovement()->GravityScale = 0.8f;
		bIsHoldingJump = false;
	}
}

void AAltheaPlayer::ReleaseJump()
{
	TimeHoldingJump = 0;
	bIsHoldingJump = false;
	bStartedGliding = false;
	GetCharacterMovement()->GravityScale = 0.8;
	timeSinceReleaseJump = 0;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString("Jump Released"));
}

void AAltheaPlayer::Landed(const FHitResult& hit)
{
	Super::Landed(hit);
	GetCharacterMovement()->GravityScale = 0.8;
	CurrentJumpCount = 0;
}

void AAltheaPlayer::CollectShard()
{
	ShardsCollected++;
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString("Current Shards: ") + FString::FromInt(ShardsCollected));
}

int AAltheaPlayer::TakeShards(int value)
{
	//If value is larger than what the player has
	if (value > ShardsCollected) {
		//Create a temporary variable to hold the number to return
		int tempShards = ShardsCollected;

		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString("Shards Taken: ") + FString::FromInt(tempShards));

		//Set the value to zero after storing the value to return
		ShardsCollected = 0;
		return tempShards;
	}
	//If the value they want is less than or equal to the value the player has
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString("Shards Taken: ") + FString::FromInt(ShardsCollected - value));

		ShardsCollected -= value;

		//Return the desired value
		return value;
	}
}
