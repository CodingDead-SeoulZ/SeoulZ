// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SZCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/SZCharacterControlData.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Player/SZPlayerController.h"
#include "Player/Components/SZInventoryComponent.h"
#include "Player/Components/SZQuickSlotComponent.h"
#include "Player/Components/SZInteractionComponent.h"
#include "Player/Components/SZCharacterEquipmentComponent.h"

#include "GameplayEffect.h" 

#include "AbilitySystemComponent.h"
#include "Attribute/SZAttributeSet.h"
#include "GameplayAbilitySpec.h"

#include "UI/SZWidgetComponent.h"
#include "UI/SZUserWidget.h"
#include "UI/SZHpBarUserWidget.h"

ASZCharacterPlayer::ASZCharacterPlayer()
{
	//
	PrimaryActorTick.bCanEverTick = true;

	//
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = ThirdArmLength;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = true;

	// TPS 카메라 (기존 FollowCamera 역할)
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;

	// FPS 카메라 (캐릭터 Mesh에 붙이거나, Root에 붙이고 상대위치로 조정)
	// 보통은 Mesh(Head 소켓)에 붙이는 방식이 가장 깔끔합니다.
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetMesh()); // 또는 RootComponent
	FirstPersonCamera->SetRelativeLocation(FirstPersonRelativeLocation);
	FirstPersonCamera->bUsePawnControlRotation = true;

	// 처음에는 TPS만 활성
	ThirdPersonCamera->SetActive(true);
	FirstPersonCamera->SetActive(false);

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Game/Input/Actions/IA_Move.IA_Move"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	//
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	//
	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Game/Input/Actions/IA_ChangeControl.IA_ChangeControl"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	//
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMouseLookRef(TEXT("/Game/Input/Actions/IA_MouseLook.IA_MouseLook"));
	if (nullptr != InputActionMouseLookRef.Object)
	{
		MouseLookAction = InputActionMouseLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionCrouchRef(TEXT("/Game/Input/Actions/IA_Crouch.IA_Crouch"));
	if (nullptr != InputActionCrouchRef.Object)
	{
		CrouchAction = InputActionCrouchRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRollRef(TEXT("/Game/Input/Actions/IA_Roll.IA_Roll"));
	if (nullptr != InputActionRollRef.Object)
	{
		RollAction = InputActionRollRef.Object;
	}

#pragma region 인벤토리 컴포넌트
	//인벤토리 컴포넌트
	SZInventory = CreateDefaultSubobject<USZInventoryComponent>(TEXT("SZInventory")); 
	
	//퀵 슬롯 컴포넌트
	SZQuickSlot = CreateDefaultSubobject<USZQuickSlotComponent>(TEXT("SZQuickSlot"));

	// 의상 착용 컴포넌트
	SZCharacterEquipment = CreateDefaultSubobject<USZCharacterEquipmentComponent>(TEXT("SZCharacterEquipment"));

	//
	SZInteraction = CreateDefaultSubobject<USZInteractionComponent>(TEXT("SZInteraction"));
#pragma endregion

	//ASC, AttributeSet 초기화
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<USZAttributeSet>(TEXT("AttributeSet"));

	// 체력 바 설정
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/HUD/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBarWidgetClass = HpBarWidgetRef.Class;
	}
}

void ASZCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//
	ASC->InitAbilityActorInfo(this, this);

	//
	ASC->ApplyGameplayEffectToSelf(
		PlayerInitGE.GetDefaultObject(),
		1.0f,
		ASC->MakeEffectContext()
	);

	//
	AttributeSet->InitHealth(AttributeSet->GetMaxHealth());

	//
	SZPC = Cast<ASZPlayerController>(NewController);
	if (!SZPC)
	{
		return;
	}
}

void ASZCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//
	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent called: %s"), *GetName());

	//
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent=%s"), EIC ? TEXT("VALID") : TEXT("NULL"));

	//
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//
	EnhancedInputComponent->BindAction(JumpAction,			ETriggerEvent::Triggered,	this,	&ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction,			ETriggerEvent::Completed,	this,	&ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction,			ETriggerEvent::Triggered,	this,	&ASZCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(MouseLookAction,		ETriggerEvent::Triggered,	this,	&ASZCharacterPlayer::MouseLook);
	EnhancedInputComponent->BindAction(ChangeControlAction,	ETriggerEvent::Started,		this,	&ASZCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ASZCharacterPlayer::Crouched);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ASZCharacterPlayer::Uncrouched);
	EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &ASZCharacterPlayer::Roll);

#pragma region 인벤토리
	//
	EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Started, this, &ASZCharacterPlayer::PickUp);

	// 인벤토리 열고 줍기
	EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ASZCharacterPlayer::ToggleInventory);
	// 퀵 슬롯 선택
	EnhancedInputComponent->BindAction(SelectedF1Action, ETriggerEvent::Started, this, &ASZCharacterPlayer::SelectedF1);
	EnhancedInputComponent->BindAction(SelectedF2Action, ETriggerEvent::Started, this, &ASZCharacterPlayer::SelectedF2);
	EnhancedInputComponent->BindAction(SelectedF3Action, ETriggerEvent::Started, this, &ASZCharacterPlayer::SelectedF3);
	EnhancedInputComponent->BindAction(SelectedF4Action, ETriggerEvent::Started, this, &ASZCharacterPlayer::SelectedF4);
#pragma endregion
}

void ASZCharacterPlayer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#pragma region 캐릭터 메쉬
	//
	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (!IsValid(CharacterMesh))
	{
		return;
	}

	//
	auto Equipment = [&](USkeletalMeshComponent* Follower)
		{
			//
			if (!IsValid(Follower) || Follower == CharacterMesh)
			{
				return;
			}
			//
			const bool bForceUpdate = true;
			const bool bFollowerShouldTickPose = false;

			//
			Follower->SetLeaderPoseComponent(CharacterMesh, bForceUpdate, bFollowerShouldTickPose);
			// Follower->bUpdateAnimationInEditor = false;
			// Follower->SetAnimationMode(EAnimationMode::AnimationBlueprint); 
		};

	//
	Equipment(Helmet);
	Equipment(Vest);
	Equipment(Gloves);
	//
	Equipment(Holster);
	Equipment(Magazine);
	Equipment(PrimaryGun);
	Equipment(SecondaryGun);
#pragma endregion

}

UAbilitySystemComponent* ASZCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

bool ASZCharacterPlayer::ApplyInstantGE(const TSubclassOf<UGameplayEffect>& GE, float Level)
{
	if (!ASC || !GE) 
	{
		return false;
	}

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GE, Level, Context);
	if (!SpecHandle.IsValid()) 
	{
		return false;
	}

	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	return true;
}

FActiveGameplayEffectHandle ASZCharacterPlayer::ApplyInfiniteGE(const TSubclassOf<UGameplayEffect>& GE, float Level)
{
	if (!ASC || !GE)
	{
		return FActiveGameplayEffectHandle();
	}

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GE, Level, Context);
	if (!SpecHandle.IsValid())
	{
		return FActiveGameplayEffectHandle();
	}

	// 추후 장비 장착·해제 시 사용할 핸들 반환
	const FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	return ActiveHandle;
}

void ASZCharacterPlayer::RemoveInfiniteGE(FActiveGameplayEffectHandle& Handle)
{
	if (ASC && Handle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(Handle);
		Handle.Invalidate();
	}
}

void ASZCharacterPlayer::DestroyWeapon()
{
	if (!WeaponGun) 
	{
		return;
	}

	WeaponGun->Destroy();
}

void ASZCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	//
	CurrentControlType = ECharacterControlType::ThirdPerson;
	ApplyThirdPersonSettings(true);

	//
	if (ASC != nullptr)
	{
		ASC->InitAbilityActorInfo(this, this);
		UE_LOG(LogTemp, Log, TEXT("ASC is not null"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("ASC is null"));
	}

	/*if (ASC && ASC->AbilityActorInfo.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("ASC InitAbilityActorInfo OK"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASC InitAbilityActorInfo FAILED"));
	}*/

	//
	FActiveGameplayEffectHandle Handle =
	ASC->ApplyGameplayEffectToSelf(
		PlayerInitGE.GetDefaultObject(),
		1.0f,
		ASC->MakeEffectContext()
	);

	/*if (Handle.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("GameplayEffect applied successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameplayEffect apply FAILED"));
	}*/

	//
	AttributeSet->InitHealth(AttributeSet->GetMaxHealth());

	// 테스트
	UE_LOG(LogTemp, Warning, TEXT("Member SZInteraction: %s"), SZInteraction ? TEXT("VALID") : TEXT("NULL"));

	TArray<USZInteractionComponent*> Comps;
	this->GetComponents<USZInteractionComponent>(Comps);
	UE_LOG(LogTemp, Warning, TEXT("GetComponentsByClass count: %d"), Comps.Num());
	for (auto* C : Comps)
	{
		UE_LOG(LogTemp, Warning, TEXT(" - %s"), *GetNameSafe(C));
	}

	// 체력 바 설정
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC || !PC->IsLocalController())
	{
		return; // 로컬 플레이어만 HUD 생성
	}

	if (!HpBarWidgetClass || HudHpBarWidget)
	{
		return;
	}

	// 1) 뷰포트 HUD 생성
	UUserWidget* Created = CreateWidget<UUserWidget>(PC, HpBarWidgetClass);
	HudHpBarWidget = Cast<USZHpBarUserWidget>(Created);
	if (!HudHpBarWidget)
	{
		return;
	}

	HudHpBarWidget->AddToViewport(0);

	// 2) 좌상단 고정(UMG 앵커를 안 만져도 코드로 고정 가능)
	HudHpBarWidget->SetAnchorsInViewport(FAnchors(0.f, 0.f));
	HudHpBarWidget->SetAlignmentInViewport(FVector2D(0.f, 0.f));
	HudHpBarWidget->SetPositionInViewport(FVector2D(20.f, 20.f), false);
	HudHpBarWidget->SetDesiredSizeInViewport(FVector2D(450.f, 24.f));
	HudHpBarWidget->SetRenderScale(FVector2D(1.5f, 1.5f));

	// 3) HUD 위젯에 "플레이어"를 Owner로 주입
	HudHpBarWidget->SetAbilitySystemComponent(this);
}

void ASZCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!SZInteraction)
	{
		SZInteraction = FindComponentByClass<USZInteractionComponent>();
		UE_LOG(LogTemp, Warning, TEXT("[Fixup] SZInteraction = %s"), *GetNameSafe(SZInteraction));
	}
}

void ASZCharacterPlayer::SetDead()
{
	//
	UE_LOG(LogTemp, Log, TEXT("Player is Dead"));
	//
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	//
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//
	AnimInstance->StopAllMontages(0.0f);
}

void ASZCharacterPlayer::ChangeCharacterControl()
{
	//
	const ECharacterControlType NewType =
		(CurrentControlType == ECharacterControlType::ThirdPerson) ? ECharacterControlType::FirstPerson : ECharacterControlType::ThirdPerson;

	SetCharacterControl(NewType);

}

void ASZCharacterPlayer::SetCharacterControl(ECharacterControlType NewType)
{
	if (CurrentControlType == NewType)
		return;

	AController* PC = GetController();
	const FRotator Saved = PC ? PC->GetControlRotation() : FRotator::ZeroRotator;

	CurrentControlType = NewType;

	if (NewType == ECharacterControlType::ThirdPerson)
	{
		ApplyThirdPersonSettings(true);

		// 1) 컨트롤러 회전 복원
		if (PC)
		{
			PC->SetControlRotation(Saved);
		}

		// 2) ★ 몸(하체)도 컨트롤러 Yaw로 즉시 정렬
		if (PC)
		{
			const float Third_Yaw = PC->GetControlRotation().Yaw;
			SetActorRotation(FRotator(0.f, Third_Yaw, 0.f));
		}
	}
	else
	{
		ApplyFirstPersonSettings(true);

		if (PC)
		{
			PC->SetControlRotation(Saved);
		}

		// (FP는 어차피 UseControllerRotationYaw=true라 보통 추가 정렬 불필요)
	}
}

void ASZCharacterPlayer::ApplyThirdPersonSettings(bool bInstant)
{
	FirstPersonCamera->SetActive(false);
	ThirdPersonCamera->SetActive(true);

	CameraBoom->TargetArmLength = ThirdArmLength;
	CameraBoom->SocketOffset = ThirdSocketOffset;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = true;

	UCharacterMovementComponent* Move = GetCharacterMovement();

	// 3인칭에서도 카메라(Yaw) = 캐릭터(Yaw)로 동기화
	bUseControllerRotationYaw = true;
	Move->bOrientRotationToMovement = false;
	Move->bUseControllerDesiredRotation = true;
	Move->RotationRate = FRotator(0.f, 720.f, 0.f);
}

void ASZCharacterPlayer::ApplyFirstPersonSettings(bool bInstant)
{
	ThirdPersonCamera->SetActive(false);
	FirstPersonCamera->SetActive(true);

	CameraBoom->TargetArmLength = 0.f;
	CameraBoom->SocketOffset = FVector::ZeroVector;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bDoCollisionTest = false;

	UCharacterMovementComponent* Move = GetCharacterMovement();

	bUseControllerRotationYaw = true;
	Move->bOrientRotationToMovement = false;
	Move->bUseControllerDesiredRotation = true;
	Move->RotationRate = FRotator(0.f, 720.f, 0.f);
}

void ASZCharacterPlayer::SetCharacterControlData(const USZCharacterControlData* CharacterControlData)
{
	//
	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void ASZCharacterPlayer::Move(const FInputActionValue& Value)
{
	//
	FVector2D MovementVector = Value.Get<FVector2D>();

	//
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	//
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ASZCharacterPlayer::MouseLook(const FInputActionValue& Value)
{
	//
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	//
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ASZCharacterPlayer::Crouched(const FInputActionValue& Value)
{
	Super::Crouch();
}

void ASZCharacterPlayer::Uncrouched(const FInputActionValue& Value)
{
	Super::UnCrouch();
}


void ASZCharacterPlayer::Roll(const FInputActionValue& Value)
{
	const UCharacterMovementComponent* Move = GetCharacterMovement();
	if (!Move || Move->MovementMode != MOVE_Walking)
		return;

	if (bIsRolling) return;
	if (!RollMontage) return;

	const float Speed2D = GetVelocity().Size2D();
	constexpr float MinRollSpeed = 200.f;

	if (Speed2D < MinRollSpeed)
	{
		// Idle 상태(또는 거의 정지)면 롤 금지
		return;
	}

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (!AnimInst) return;

	bIsRolling = true;

	AnimInst->Montage_Play(RollMontage, 2.0f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ASZCharacterPlayer::OnRollMontageEnded);
	AnimInst->Montage_SetEndDelegate(EndDelegate, RollMontage);
}

void ASZCharacterPlayer::OnRollMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == RollMontage)
	{
		bIsRolling = false;
	}
}

#pragma region 인벤토리
void ASZCharacterPlayer::PickUp(const FInputActionValue& Value)
{
	if (SZInteraction) 
	{
		SZInteraction->PickUpItem();
	}
}

void ASZCharacterPlayer::ToggleInventory(const FInputActionValue& Value)
{
	if (SZPC)
	{
		SZPC->ToggleInventory();
	}
}

void ASZCharacterPlayer::SelectedF1(const FInputActionValue& Value)
{
	if (SZQuickSlot)
	{
		SZQuickSlot->GetSelectedSlot(0);
	}
}

void ASZCharacterPlayer::SelectedF2(const FInputActionValue& Value)
{
	if (SZQuickSlot)
	{
		SZQuickSlot->GetSelectedSlot(1);
	}
}

void ASZCharacterPlayer::SelectedF3(const FInputActionValue& Value)
{
	if (SZQuickSlot)
	{
		SZQuickSlot->GetSelectedSlot(2);
	}
}

void ASZCharacterPlayer::SelectedF4(const FInputActionValue& Value)
{
	if (SZQuickSlot)
	{
		SZQuickSlot->GetSelectedSlot(3);
	}
}
#pragma endregion


