// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SZPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/SZWardrobe.h"
#include "UI/Inventory/SZInventoryActor.h"
#include "UI/Inventory/SZPlayerHud.h"

void ASZPlayerController::ShowPlayerHud()
{
	if (PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<USZPlayerHud>(this, PlayerHUDClass);

		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
		}
	}
}

//void ASZPlayerController::ToggleInventory()
//{
//	if (IsValid(InventoryActor))
//	{
//		InventoryActor->CloseInventory();
//	}
//	else
//	{
//		InventoryActor = CreateInventoryActor();
//
//		if (IsValid(InventoryActor))
//		{
//			InventoryActor->OpenInventory();
//		}
//	}
//}

void ASZPlayerController::ToggleInventory()
{
	if (!IsValid(InventoryActor))
	{
		InventoryActor = CreateInventoryActor();
		if (!IsValid(InventoryActor)) 
		{
			return;
		}

		bInventoryOpen = false;
	}

	bInventoryOpen = !bInventoryOpen;
	if (bInventoryOpen)
	{
		InventoryActor->OpenInventory();
	}
	else
	{
		InventoryActor->CloseInventory();
	}
}


FSpawnWardrobeResult ASZPlayerController::CreateWardrobeActor()
{
	FSpawnWardrobeResult Out;

	const FVector SpawnLoc(0.f, 0.f, 1000000.f);
	const FRotator SpawnRot(0.f, 0.f, 0.f);
	const FTransform SpawnTM(SpawnRot, SpawnLoc, FVector(1.f));

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = GetPawn();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (!WardrobeClass)
	{
		return Out;
	}

	ASZWardrobe* Spawned = GetWorld()->SpawnActor<ASZWardrobe>(WardrobeClass, SpawnTM, Params);
	WardrobeActor = Spawned;

	Out.Transform = SpawnTM;
	Out.WardrobeActor = Spawned;
	return Out;
}

ASZInventoryActor* ASZPlayerController::CreateInventoryActor()
{
	if (!IsValid(WardrobeActor))
	{
		CreateWardrobeActor();
	}

	const FTransform SpawnTM = WardrobeActor->GetActorTransform();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = GetPawn();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (!InventoryActorClass)
	{
		return nullptr;
	}

	ASZInventoryActor* Spawned =
		GetWorld()->SpawnActorDeferred<ASZInventoryActor>(InventoryActorClass, SpawnTM, nullptr, nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (!IsValid(Spawned))
	{
		return nullptr;
	}

	Spawned->WardrobeActor = WardrobeActor;
	Spawned->FinishSpawning(SpawnTM);
	InventoryActor = Spawned;

	return Spawned;
}

void ASZPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ShowPlayerHud();

	// 인게임 레벨에서 확실히 게임 입력으로 전환하고 싶다면 여기서 호출
	ApplyGameInputMode();
}

void ASZPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CurrentCharacterContext, 0);
		}
	}
}

void ASZPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Possess 타이밍이 늦는 프로젝트도 있어 OnPossess에서 한 번 더 고정하면 안정적
	ApplyGameInputMode();
}

void ASZPlayerController::ApplyGameInputMode()
{
	// UI Only/포커스 문제 정리
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	bShowMouseCursor = false;

	// 혹시 로비에서 잠가둔 경우 대비
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);
}

void ASZPlayerController::ApplyUIOnlyMode(UUserWidget* FocusWidget)
{
	FInputModeUIOnly InputMode;

	if (FocusWidget)
	{
		InputMode.SetWidgetToFocus(FocusWidget->TakeWidget());
	}

	// 마우스 잠금 정책은 프로젝트 취향에 따라
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetInputMode(InputMode);
	bShowMouseCursor = true;

	// UI 전용이면 보통 이동/시점도 잠금
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);
}
