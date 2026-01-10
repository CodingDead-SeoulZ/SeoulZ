// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundBase.h"
#include "SZAchievementSection.generated.h"

//﻿---------------------------------------------------------------------------------------------------------
// Author		: 안재범                                                                                       
// Date			: 2026-01-08                                                                                
// Copyright	: CodingDead                                                                                
// Description	: 태그에 해당하는 업적들만 표시되게 하는 위젯 클래스.                                                                         
//				                                                                                            
//----------------------------------------------------------------------------------------------------------

class UButton;
class UScrollBox;

// 태그 목록.
UENUM(BlueprintType)
enum class ETagType : uint8
{
	None,
	Weapon,
	Time,
	Combat
};

// 태그 선택 델리게이트 선언.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementTagSelected, ETagType, TagType);

UCLASS()
class PROJECTSEOULZ_API USZAchievementSection : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	// 태그 선택 이벤트.
	UPROPERTY(BlueprintAssignable, Category="Achievement")
	FOnAchievementTagSelected OnAchievementTagSelected;
	
	void NotifyTagSelected(ETagType TagType);
	
	// 버튼 클릭 바인드.
	UFUNCTION()
	void OnAllClicked();
		
	UFUNCTION()
	void OnWeaponClicked();
	
	UFUNCTION()
	void OnTimeClicked();
	
	UFUNCTION()
	void OnCombatClicked();
	
protected:
	// 버튼
	UPROPERTY(meta=(BindWidget))
	UButton* ButtonAll;

	UPROPERTY(meta=(BindWidget))
	UButton* ButtonWeapon;

	UPROPERTY(meta=(BindWidget))
	UButton* ButtonTime;

	UPROPERTY(meta=(BindWidget))
	UButton* ButtonCombat;
		
	// 현재 활성화된 태그.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Achievement|State")
	ETagType CurrentTag;
	
	// Button 클릭 사운드
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* TagButtonSound;
};
