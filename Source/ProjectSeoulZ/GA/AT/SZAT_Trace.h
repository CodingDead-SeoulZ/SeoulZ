// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "SZAT_Trace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTraceResultDelegate);

//---------------------------------------------------------------------------------------------------------
// Author       : 
// Date         : 2025-12-00
// Copyright    : 
//
// Description  : 
//                
//                
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API USZAT_Trace : public UAbilityTask
{
	GENERATED_BODY()

public:
	USZAT_Trace();

	//
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static USZAT_Trace* CreateTask(UGameplayAbility* OwningAbility);

	//
	virtual void Activate() override;
	//
	virtual void OnDestroy(bool AbilityEnded) override;

public:
	//
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;
	
};
