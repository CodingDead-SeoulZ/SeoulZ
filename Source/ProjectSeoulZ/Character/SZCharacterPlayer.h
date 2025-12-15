// Fill out your copyright notice in the Description page of Project Settings.

//---------------------------------------------------------------------------------------------------------
// Author       : 박지훈
// Date       : 2025-12-15
// Copyright   : 
//
// Description : CharacterPlayer 클래스 
//               플레이어의 전반적인 동작을 관할하는 클래스
//				 
//                 
//----------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Character/SZCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "SZCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API ASZCharacterPlayer : public ASZCharacterBase
{
	GENERATED_BODY()
	
public:
	ASZCharacterPlayer();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// 캐릭터 게임 시작 및 죽음
	virtual void BeginPlay() override;
	virtual void SetDead() override;

	// 캐릭터 컨트롤
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	/*virtual void SetCharacterControlData(const class USZCharacterControlData* CharacterControlData) override;*/

	// 카메라
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// 입력 맵핑
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ThirdMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ThirdLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FirstMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FirstLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkillAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void ThirdMove(const FInputActionValue& Value);
	void ThirdLook(const FInputActionValue& Value);

	void FirstMove(const FInputActionValue& Value);
	void FirstLook(const FInputActionValue& Value);


	ECharacterControlType CurrentCharacterControlType;


	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
