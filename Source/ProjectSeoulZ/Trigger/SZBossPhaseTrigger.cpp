// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/SZBossPhaseTrigger.h"
#include "Components/ShapeComponent.h"
#include "Player/SZCharacterPlayer.h"
#include "GameMode/SZGameModeBase.h"
#include "GameMode/SZGameInstance.h"
#include <Kismet/GameplayStatics.h>

ASZBossPhaseTrigger::ASZBossPhaseTrigger()
{
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASZBossPhaseTrigger::OnOverlapBegin);
}

void ASZBossPhaseTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bLevelOpened)
		return;

	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(OtherActor);

	if ( Player && OtherActor != this)
	{
		UE_LOG(LogTemp,Log, TEXT("BeginOverlap"));

		UWorld* World = GetWorld();

		ASZGameModeBase* GameMode = Cast<ASZGameModeBase>(World->GetAuthGameMode());

		if (GameMode->GetMonsterCount() == 0 && !bLevelOpened)
		{
			bLevelOpened = true;

			GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//Trigger는 "GM에 위임"만
			GameMode->HandleBossPhaseTrigger();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Monster is Alive"));
		}
	}
}
