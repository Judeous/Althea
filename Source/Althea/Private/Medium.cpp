// Fill out your copyright notice in the Description page of Project Settings.


#include "Medium.h"
#include "Components/SphereComponent.h"
#include "AltheaPlayer.h"
#include "Containers/UnrealString.h"

// Sets default values
AMedium::AMedium()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	BloomRadius = CreateDefaultSubobject<USphereComponent>("BloomRadius");
}

// Called when the game starts or when spawned
void AMedium::BeginPlay()
{
	Super::BeginPlay();

	BloomRadius->SetWorldLocation(CollisionSphere->GetComponentLocation());
	DesiredShardsLeft = DesiredShards;

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMedium::OnComponentBeginOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AMedium::OnComponentEndOverlap);

	if (bBloomsEverything) {
		Tags.Add("BloomsEverything");
	}

	//Store a temporary variable to construct the areaCode tag that BloomableObjects check for
	FString areaCode = "Area" + areaCode.FromInt(Area);
	//Add the areaCode tag to itself
	Tags.Add(FName(areaCode));
}

// Called every frame
void AMedium::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//If displaing the CounterDisplay and both the CounterDisplay and APlayer are not null
	if (bIsDisplayingCounter && CounterDisplay->IsValidLowLevel() && Aplayer != NULL) {
		CounterDisplay->DisplayCounter(Aplayer->GetActorLocation(), GetActorLocation(), DesiredShards - DesiredShardsLeft, DesiredShards, DeltaTime);
	}
	//If the player has given all the desired gems and the BloomRadius is not too big
	if (bGrantedAbility && BloomRadius->GetComponentScale().Size() < maxBloomRadius) {
		//Expand the BloomRadius
		//BloomRadius->SetSphereRadius(BloomRadius->GetScaledSphereRadius() + 1);
		BloomRadius->SetWorldScale3D({ 
			BloomRadius->GetComponentScale().X + BloomExpansionSpeed, 
			BloomRadius->GetComponentScale().Y + BloomExpansionSpeed, 
			BloomRadius->GetComponentScale().Z + BloomExpansionSpeed });
	}
}

void AMedium::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsDisplayingCounter = true;
	//If the thing that overlapped the radius is a player
	if (AAltheaPlayer* player = Cast<AAltheaPlayer>(OtherActor)) {
		Aplayer = player;
		//If the ability hasn't been granted yet
		if (!bGrantedAbility) {
			//Take the desired number of Shards
			DesiredShardsLeft -= player->TakeShards(DesiredShardsLeft);
			//If there's one Shard left to take
			if (DesiredShardsLeft == 1) {
				DesiredShardsLeft -= player->TakeShards(1);
			}
			//If the player has brought enough Shards
			if (DesiredShardsLeft == 0) {
				//Give an ability
				if (bGivesBloom) {
					player->SetHasBloom(true);
				}
				else if (bGivesDoubleJump) {
					player->SetHasDoubleJump(true);
				}
				else if (bGivesGlide) {
					player->SetHasGlide(true);
				}
				bGrantedAbility = true;
			}
		}
		//If the CounterDisplay is not null
		//if (CounterDisplay->IsValidLowLevel())
		//	CounterDisplay->DisplayCounter(OtherActor->GetActorLocation(), GetActorLocation(), DesiredShards - DesiredShardsLeft, DesiredShards);
	}
}

void AMedium::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsDisplayingCounter = false;
	CounterDisplay->Hide();
}