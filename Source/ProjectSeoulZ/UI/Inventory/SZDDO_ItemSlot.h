// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SZDDO_ItemSlot.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-27
// Copyright    : 
//
// Description  : 드래그앤드랍 Operation
//               드래그 중에 들고 다니는 데이터 상자
//                 
//----------------------------------------------------------------------------------------------------------

class USZInventoryBaseComponent;

UCLASS()
class PROJECTSEOULZ_API USZDDO_ItemSlot : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Component", meta = (ExposeOnSpawn = "true"))
	TObjectPtr<USZInventoryBaseComponent> SourceInventory;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Index", meta = (ExposeOnSpawn = "true"))
	int32 SourceIndex;
};
