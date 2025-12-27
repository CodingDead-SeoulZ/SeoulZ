// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SZSelectedSlot.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-27
// Copyright    : 
//
// Description  : QuickSlot 선택 효과용 위젯
//             
//                 
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API USZSelectedSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Index", meta = (ExposeOnSpawn = "true"))
	int32 Index;
};
