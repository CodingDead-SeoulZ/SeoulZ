// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SZItemSlotPreview.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-27
// Copyright    : 
//
// Description  : Item Slot Preview
//               아이템 이동 화면
//                 
//----------------------------------------------------------------------------------------------------------

class UImage;
class UTextBlock;

UCLASS()
class PROJECTSEOULZ_API USZItemSlotPreview : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Img_Item;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_StackCount;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ItemID", meta = (ExposeOnSpawn = "true"))
	FName ItemID;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "StackCount", meta = (ExposeOnSpawn = "true"))
	int32 StackCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	TObjectPtr<UDataTable> ItemTable;
};
