// Fill out your copyright notice in the Description page of Project Settings.


#include "MushroomBouncepad.h"
#include "Components/SphereComponent.h"
#include <Althea/Public/AltheaPlayer.h>

// Sets default values
AMushroomBouncepad::AMushroomBouncepad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	BounceSphere = CreateDefaultSubobject<USphereComponent>("BounceSphere");
}

void AMushroomBouncepad::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AAltheaPlayer* player = Cast<AAltheaPlayer>(OtherActor)) {
		player->LaunchCharacter({ 0, 0, 1200 }, false, true);
	}
}

// Called when the game starts or when spawned
void AMushroomBouncepad::BeginPlay()
{
	Super::BeginPlay();
	
	BounceSphere->OnComponentBeginOverlap.AddDynamic(this, &AMushroomBouncepad::OnComponentBeginOverlap);
	//Move the sphere up to the top of the mushroom
	BounceSphere->SetWorldLocation(FVector(BounceSphere->GetComponentLocation().X,
		BounceSphere->GetComponentLocation().Y,
		BounceSphere->GetComponentLocation().Z + 30));
	//No wark
	//BounceSphere->SetWorldScale3D(FVector(30, 30, 30));
}

// Called every frame
void AMushroomBouncepad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

