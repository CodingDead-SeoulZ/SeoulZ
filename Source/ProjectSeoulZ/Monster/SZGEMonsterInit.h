// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "SZGEMonsterInit.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API USZGEMonsterInit : public UGameplayEffect
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InitTag")
	FGameplayTag Tag;
};
