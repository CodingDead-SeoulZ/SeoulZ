// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZItemActionUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void USZItemActionUI::NativePreConstruct()
{
	Txt_txt->SetText(ActionTxt);
}

void USZItemActionUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_ItemAction)
	{
		Button_ItemAction->OnHovered.AddDynamic(this, &USZItemActionUI::OnHovered);
		Button_ItemAction->OnUnhovered.AddDynamic(this, &USZItemActionUI::OnUnHovered);
		Button_ItemAction->OnClicked.AddDynamic(this, &USZItemActionUI::HandleInternalClicked);
	}
}

void USZItemActionUI::OnHovered() 
{
	if (Txt_txt)
	{
		Txt_txt->SetColorAndOpacity(FSlateColor(FLinearColor::FromSRGBColor(FColor(0x78, 0x78, 0x78, 0xFF))));
	}
}

void USZItemActionUI::OnUnHovered() 
{
	if (Txt_txt)
	{
		Txt_txt->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
}

void USZItemActionUI::HandleInternalClicked()
{
	OnItemActionClicked.Broadcast();
}