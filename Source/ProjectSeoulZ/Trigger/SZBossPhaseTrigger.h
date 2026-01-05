// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SZBossPhaseTrigger.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API ASZBossPhaseTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	ASZBossPhaseTrigger();

protected:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
