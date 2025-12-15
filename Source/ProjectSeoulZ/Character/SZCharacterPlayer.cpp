// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SZCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SZCharacterControlData.h"

ASZCharacterPlayer::ASZCharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/Input/IMC_Default.IMC_Default"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Game/Input/Actions/IA_Move.IA_Move"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Game/Input/Actions/IA_Look.IA_Look"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	//static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	//if (nullptr != InputChangeActionControlRef.Object)
	//{
	//	ChangeControlAction = InputChangeActionControlRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UInputAction> InputActionThirdMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	//if (nullptr != InputActionThirdMoveRef.Object)
	//{
	//	ThirdMoveAction = InputActionThirdMoveRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UInputAction> InputActionThirdLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ThirdLook.IA_ThirdLook'"));
	//if (nullptr != InputActionThirdLookRef.Object)
	//{
	//	ThirdLookAction = InputActionThirdLookRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFirstMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	//if (nullptr != InputActionFirstMoveRef.Object)
	//{
	//	FirstMoveAction = InputActionFirstMoveRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFirstLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	//if (nullptr != InputActionFirstLookRef.Object)
	//{
	//	FirstLookAction = InputActionFirstLookRef.Object;
	//}

}

void ASZCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);


	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASZCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASZCharacterPlayer::Look);

	//EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &ASZCharacterPlayer::ChangeCharacterControl);
	//EnhancedInputComponent->BindAction(ThirdMoveAction, ETriggerEvent::Triggered, this, &ASZCharacterPlayer::ThirdMove);
	//EnhancedInputComponent->BindAction(ThirdLookAction, ETriggerEvent::Triggered, this, &ASZCharacterPlayer::ThirdLook);
	//EnhancedInputComponent->BindAction(FirstMoveAction, ETriggerEvent::Triggered, this, &ASZCharacterPlayer::FirstMove);
	//EnhancedInputComponent->BindAction(FirstLookAction, ETriggerEvent::Triggered, this, &ASZCharacterPlayer::FirstLook);
}

void ASZCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		//Subsystem->RemoveMappingContext(DefaultMappingContext);
	}
}

void ASZCharacterPlayer::SetDead()
{
}

void ASZCharacterPlayer::ChangeCharacterControl()
{
}

void ASZCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
}

//void ASZCharacterPlayer::SetCharacterControlData(const USZCharacterControlData* CharacterControlData)
//{
//	Super::SetCharacterControlData(CharacterControlData);
//}

void ASZCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ASZCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ASZCharacterPlayer::ThirdMove(const FInputActionValue& Value)
{
}

void ASZCharacterPlayer::ThirdLook(const FInputActionValue& Value)
{
}

void ASZCharacterPlayer::FirstMove(const FInputActionValue& Value)
{
}

void ASZCharacterPlayer::FirstLook(const FInputActionValue& Value)
{
}

UAbilitySystemComponent* ASZCharacterPlayer::GetAbilitySystemComponent() const
{
	return nullptr;
}
