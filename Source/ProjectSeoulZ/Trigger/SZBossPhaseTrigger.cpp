// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/SZBossPhaseTrigger.h"
#include "Components/ShapeComponent.h"
#include "Player/SZCharacterPlayer.h"
#include "GameMode/SZGameModeBase.h"
#include <Kismet/GameplayStatics.h>

ASZBossPhaseTrigger::ASZBossPhaseTrigger()
{
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASZBossPhaseTrigger::OnOverlapBegin);
}

void ASZBossPhaseTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(OtherActor);

	if ( Player && OtherActor != this)
	{
		UE_LOG(LogTemp,Log, TEXT("BeginOverlap"));

		UWorld* World = GetWorld();

		ASZGameModeBase* GameMode = Cast<ASZGameModeBase>(World->GetAuthGameMode());

		if (GameMode->GetMonsterCount() == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("BossPhase"));
			UGameplayStatics::OpenLevel(World, FName("/Game/Map/BossPhaseTestMap.BossPhaseTestMap"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Monster is Alive"));
		}
	}
}
