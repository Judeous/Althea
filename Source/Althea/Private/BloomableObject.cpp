// Fill out your copyright notice in the Description page of Project Settings.

#include "BloomableObject.h"
#include "Components/SphereComponent.h"
#include "AltheaPlayer.h"

// Sets default values
ABloomableObject::ABloomableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	//LivingMesh = CreateDefaultSubobject<UStaticMeshComponent>("LivingMesh");
	//DeadMesh = CreateDefaultSubobject<UStaticMeshComponent>("DeadMesh");
	
	//RootComponent = CollisionSphere;
	CollisionSphere->AttachTo(RootComponent);
	//LivingMesh->AttachTo(RootComponent);
	//DeadMesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ABloomableObject::BeginPlay()
{
	Super::BeginPlay();
	//Move the meshes to the correct location so they're not centered on the origin
	//I do not know why dragging a blueprint out centers the mesh on the origin
	//if (!bIsAreaBarrier) {
	//	DeadMesh->SetWorldLocation(FVector(CollisionSphere->GetComponentLocation().X,
	//		CollisionSphere->GetComponentLocation().Y,
	//		CollisionSphere->GetComponentLocation().Z - 100));
	//	LivingMesh->SetWorldLocation(FVector(CollisionSphere->GetComponentLocation().X,
	//		CollisionSphere->GetComponentLocation().Y,
	//		CollisionSphere->GetComponentLocation().Z - 100));
	//}
	
	//Make the Dead Mesh what originally shows up in the level
	//DeadMesh->SetVisibility(true);
	//LivingMesh->SetVisibility(false);

	//CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABloomableObject::OnComponentBeginOverlap);

	//If the object does not have a custom rotation
	//if (CollisionSphere->GetComponentRotation().IsZero()) {
	//	//Add a random rotation so the bloomable objects are not all facing the same direction
	//	//as long as it's not an Area Barrier; if it is then prevent the Dead Mesh from being rotated
	//	if (!bIsAreaBarrier)
	//		DeadMesh->AddLocalRotation(FRotator(0, GetActorRotation().Yaw + FMath::RandRange(0, 359), 0));
	//	LivingMesh->AddLocalRotation(FRotator(0, GetActorRotation().Yaw + FMath::RandRange(0, 359), 0));
	//}
	////If the object does not have a custom scale
	//if (CollisionSphere->GetShapeScale() == 1) {
	//	//Randomize the scale a very small amount so they're not all the same size
	//	//as long as it's not an Area Barrier; if it is then prevent the Dead Mesh from being scaled
	//	if (!bIsAreaBarrier)
	//		DeadMesh->SetWorldScale3D(FVector(FMath::RandRange(2.80f, 3.20f), FMath::RandRange(2.30f, 3.70f), FMath::RandRange(2.80f, 4.80f)));
	//	LivingMesh->SetWorldScale3D(FVector(FMath::RandRange(2.80f, 3.20f), FMath::RandRange(2.30f, 3.70f), FMath::RandRange(2.80f, 4.70f)));
	//}
}

//void ABloomableObject::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	//Store a temporary variable because Unreal is stupid
//	FString areaCode = "Area" + areaCode.FromInt(Area);
//
//	//If the thing that overlapped blooms everything, Bloom
//	if (OtherActor->Tags.Contains("BloomsEverything")) {
//		Bloom();
//	}
//	//If this is not an Area Barrier
//	else if (!bIsAreaBarrier) {
//		//If the thing that overlapped the radius is a player
//		if (AAltheaPlayer* player = Cast<AAltheaPlayer>(OtherActor)) {
//			//If the player has Bloom
//			if (player->GetHasBloom()) {
//				Bloom();
//			}
//		}
//	}
//	//If the thing that overlapped the radius is this area's Medium's healing zone
//	if (OtherActor->Tags.Contains("Bloom") && OtherActor->Tags.Contains((FName)areaCode)) {
//		Bloom();
//	}
//}
//
//void ABloomableObject::Bloom()
//{
//	DeadMesh->SetVisibility(false);
//	LivingMesh->SetVisibility(true);
//
//	if (bIsAreaBarrier) {
//		DeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	}
//}
