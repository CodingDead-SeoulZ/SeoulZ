// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZItemCategory.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void USZItemCategory::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (Txt_FilterName)
	{
		Txt_FilterName->SetText(FilterName);
	}
}

void USZItemCategory::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Filter)
	{
		Button_Filter->OnClicked.AddDynamic(this, &USZItemCategory::HandleButtonClicked);
	}
}

void USZItemCategory::HandleButtonClicked()
{
	bIsOn = !bIsOn;

	const FLinearColor TargetColor = bIsOn ? Button_On : Button_Off;
	Button_Filter->SetBackgroundColor(TargetColor);

	OnFilterClicked.Broadcast(bIsOn, FilterID);
}