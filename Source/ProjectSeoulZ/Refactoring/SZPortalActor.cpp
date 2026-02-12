// Fill out your copyright notice in the Description page of Project Settings.


#include "Refactoring/SZPortalActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/SZCharacterPlayer.h"
#include "Refactoring/SZPortalSubsystem.h"

// Sets default values
ASZPortalActor::ASZPortalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Root);

	Trigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ASZPortalActor::BeginPlay()
{
	Super::BeginPlay();
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASZPortalActor::OnTriggerBeginOverlap);
}

// Called every frame
void ASZPortalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASZPortalActor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(OtherActor);
	if (!Player) { return; }

	if (UGameInstance* GI = GetGameInstance()) {
		if (USZPortalSubsystem* PortalSystem = GI->GetSubsystem<USZPortalSubsystem>()) {
			PortalSystem->RequestTravel(this);
		}
	}
}

