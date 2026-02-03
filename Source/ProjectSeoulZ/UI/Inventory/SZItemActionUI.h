// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SZItemActionUI.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-17
// Copyright    : Coding Dead
//
// Description  : Item 툴킷의 버튼 UI
//               예를 들어, '사용', '내려놓기' 등
//           
//----------------------------------------------------------------------------------------------------------

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSZItemActionClicked);

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API USZItemActionUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnHovered();

	UFUNCTION()
	void OnUnHovered();

	UFUNCTION()
	void HandleInternalClicked();

public:
	UPROPERTY(BlueprintAssignable)
	FOnSZItemActionClicked OnItemActionClicked;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ItemAction;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_txt;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	FText ActionTxt;
};
