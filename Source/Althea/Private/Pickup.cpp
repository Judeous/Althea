// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Math/UnrealMathUtility.h"
#include "AltheaPlayer.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("SphereCollision");

	bool positiveRotation = FMath::RandBool();
	YawRotationSpeed = positiveRotation ? FMath::RandRange(-21, 10) : FMath::RandRange(10, 21);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnComponentBeginOverlap);
	HoverLength = FMath::RandRange(1, 3);
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//First multiplication is time it takes to switch between up and down, second is speed it moves in that direction
	SetActorLocation(FVector(GetActorLocation().X,
		GetActorLocation().Y,
		GetActorLocation().Z + ((FMath::Sin(GetGameTimeSinceCreation() * HoverLength)) * 0.40f)));

	FRotator rotation = GetActorRotation();
	//rotation.Pitch += PitchRotation * DeltaTime;
	rotation.Yaw += YawRotationSpeed * DeltaTime;
	//rotation.Roll += RollRotation * DeltaTime;
	SetActorRotation(rotation);
}

void APickup::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//If the thing that overlapped the component is the player
	if (AAltheaPlayer* player = Cast<AAltheaPlayer>(OtherActor)) {
		//Call the player's CollectShard function then destroy this object to make it vanish
		player->CollectShard();
	}
}
