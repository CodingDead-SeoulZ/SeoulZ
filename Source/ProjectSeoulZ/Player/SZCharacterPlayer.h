// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Character/SZCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "GameplayEffectTypes.h"
#include "SZCharacterPlayer.generated.h"

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

class ASZPlayerController;
class USZInventoryComponent;
class USZQuickSlotComponent;
class USZInteractionComponent;
class USZCharacterEquipmentComponent;

class UGameplayEffect;                
class UAbilitySystemComponent;

//
UENUM()
enum class ECharacterControlType : uint8
{
	ThirdPerson,
	FirstPerson
};

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API ASZCharacterPlayer : public ASZCharacterBase
{
	GENERATED_BODY()
	
public:
	ASZCharacterPlayer();

	//
	virtual void PossessedBy(AController* NewController) override;
	//
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//
	virtual void OnConstruction(const FTransform& Transform) override;

	//
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

#pragma region 인벤토리 함수

	FORCEINLINE const TObjectPtr<USZInventoryComponent>				GetInventoryComponent() const { return SZInventory; }
	FORCEINLINE const TObjectPtr<USZQuickSlotComponent>				GetQuickSlotComponent() const { return SZQuickSlot; }
	FORCEINLINE const TObjectPtr<USZCharacterEquipmentComponent>	GetEquipmentComponent() const { return SZCharacterEquipment; }
	FORCEINLINE const TObjectPtr<USZInteractionComponent>			GetInteractionComponent() const { return SZInteraction; }

	// InstantGE 사용
	bool ApplyInstantGE(const TSubclassOf<UGameplayEffect>& GE, const float Level);

	// InfiniteGE 사용
	FActiveGameplayEffectHandle ApplyInfiniteGE(const TSubclassOf<UGameplayEffect>& GE, float Level);
	// InfiniteGE 제거
	void RemoveInfiniteGE(FActiveGameplayEffectHandle& Handle);

#pragma endregion

protected:
	// 캐릭터 게임 시작 및 죽음
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void SetDead() override;

	// 캐릭터 컨트롤
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	void SetCharacterControlData(const class USZCharacterControlData* CharacterControlData);

	// 카메라/이동 세팅 적용 (필요 시 분리)
	void ApplyThirdPersonSettings(bool bInstant = false);
	void ApplyFirstPersonSettings(bool bInstant = false);

protected:
	// 카메라
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Mode")
	ECharacterControlType CurrentControlType = ECharacterControlType::ThirdPerson;

	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// TPS 카메라 (현재 FollowCamera를 TPS로 쓰는 편이 자연스러움)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> ThirdPersonCamera;

	// FPS 카메라 (캐릭터 머리/눈 위치에 부착)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	// 3인칭 파라미터
	UPROPERTY(EditAnywhere, Category = "Camera|Third")
	float ThirdArmLength = 400.f;

	//
	UPROPERTY(EditAnywhere, Category = "Camera|Third")
	FVector ThirdSocketOffset = FVector(0.f, 60.f, 70.f); // 어깨 카메라 느낌

	// 1인칭 파라미터
	UPROPERTY(EditAnywhere, Category = "Camera|First")
	FVector FirstPersonRelativeLocation = FVector(0.f, 0.f, 64.f);

	//
	UPROPERTY(EditAnywhere, Category = "Camera|Blend")
	float CameraBlendSpeed = 12.f;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;
	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;
	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MouseLookAction;
	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ThirdMoveAction;
	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ThirdLookAction;
	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FirstMoveAction;
	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FirstLookAction;
	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;
	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkillAction;

	// 쪼그려 앉기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CrouchAction;

	// 구르기 관련
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RollAction;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation|Roll", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> RollMontage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Roll")
	bool bIsRolling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Roll")
	bool bIsJumping = false;

#pragma region 인벤토리 입력 값
	// 아이템 줍기 - 입력 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> PickUpAction;

	// 인벤토리 열기 - 입력 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InventoryAction;

	// 퀵 슬롯 선택
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SelectedF1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SelectedF2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SelectedF3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SelectedF4Action;
#pragma endregion

	//
	void Move(const FInputActionValue& Value);
	void MouseLook(const FInputActionValue& Value);
	
	void Crouched(const FInputActionValue& Value);
	void Uncrouched(const FInputActionValue& Value);

	// 구르기 관련
	void Roll(const FInputActionValue& Value);
	UFUNCTION()
	void OnRollMontageEnded(UAnimMontage* Montage, bool bInterrupted);

#pragma region 인벤토리 함수
	// 아이템 줍기
	void PickUp(const FInputActionValue& Value);

	// 인벤토리 열고 닫기
	void ToggleInventory(const FInputActionValue& Value);

	// 퀵 슬롯 선택
	void SelectedF1(const FInputActionValue& Value);
	void SelectedF2(const FInputActionValue& Value);
	void SelectedF3(const FInputActionValue& Value);
	void SelectedF4(const FInputActionValue& Value);
#pragma endregion

public:
#pragma region 캐릭터 메쉬. 의상
	// 캐릭터 메쉬 컴포넌트
	// 1. 의상
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshes")
	TObjectPtr<USkeletalMeshComponent> Helmet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshes")
	TObjectPtr<USkeletalMeshComponent> Vest;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshes")
	TObjectPtr<USkeletalMeshComponent> Gloves;

	// 2. 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshes")
	TObjectPtr<USkeletalMeshComponent> Holster;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshes")
	TObjectPtr<USkeletalMeshComponent> Magazine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshes")
	TObjectPtr<USkeletalMeshComponent> PrimaryGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshes")
	TObjectPtr<USkeletalMeshComponent> SecondaryGun;
#pragma endregion

#pragma region 아이템(의상) GE Handle
	TMap<FName, TArray<FActiveGameplayEffectHandle>> GEHandles;
#pragma endregion

protected:
	// GAS
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> PlayerInitGE;

	// /Game/UI/HUD/WBP_HpBar
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HpBarWidgetClass;

	UPROPERTY()
	TObjectPtr<class USZHpBarUserWidget> HudHpBarWidget;

private:
	//
	bool bWantsBlend = false;
	float BlendAlpha = 1.0f; // 0=3인칭, 1=1인칭
	ECharacterControlType TargetControlType = ECharacterControlType::ThirdPerson;

#pragma region 플레이어 컴포넌트 및 인벤토리 컴포넌트
	// 플레이어 컨트롤러
	UPROPERTY(Transient)
	TObjectPtr<ASZPlayerController> SZPC;

	// 인벤토리 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USZInventoryComponent> SZInventory;

	// 퀵 슬롯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USZQuickSlotComponent> SZQuickSlot;

	// 의상 착용 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USZCharacterEquipmentComponent> SZCharacterEquipment;

	// 상호작용 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USZInteractionComponent> SZInteraction;
#pragma endregion
};
