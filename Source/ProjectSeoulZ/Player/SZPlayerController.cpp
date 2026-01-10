// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SZPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/SZWardrobe.h"
#include "UI/Inventory/SZInventoryActor.h"
#include "UI/Inventory/SZPlayerHud.h"
#include "Item/SZItemTemplete.h"
#include "Player/SZCharacterPlayer.h"
#include "Player/Components/SZInventoryBaseComponent.h"
#include "Player/Components/SZCharacterEquipmentComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	UE_LOG(LogTemp, Warning, TEXT("ASZPlayerController::SetupInputComponent called"));

	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CurrentCharacterContext, 0);
		}
	}
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(IA_Pause, ETriggerEvent::Started, this, &ASZPlayerController::TogglePauseMenu);
}

void ASZPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

#pragma region 델리게이트 수신
	// 플레이어의 컴포넌트를 가져옴
	ASZCharacterPlayer* SZPlayer = Cast<ASZCharacterPlayer>(GetCharacter());
	if (!SZPlayer)
	{
		return;
	}

	USZInventoryBaseComponent* SZInventoryBase = SZPlayer->FindComponentByClass<USZInventoryBaseComponent>();
	if (!SZInventoryBase)
	{
		return;
	}
	
	// 옷장에서 아이템 장착
	// SZInventoryBase->OnWardrobeActorChanged.AddDynamic(this, &ASZPlayerController::UpdateWardrobe);
	// 중복 바인딩 방지. 이미 같은 항목이 바인딩돼 있으면 추가하지 않음.
	SZInventoryBase->OnWardrobeEquipped.AddUniqueDynamic(this, &ASZPlayerController::WardrobeEquipped);

	// 옷장에서 아이템(장착템) 해제
	UE_LOG(LogTemp, Warning, TEXT("[Bind] InventoryBase=%s (%p)"),
		*GetNameSafe(SZInventoryBase), SZInventoryBase);

	USZCharacterEquipmentComponent* SZEquipment = SZPlayer->GetEquipmentComponent();
	if (!SZEquipment)
	{
		return;
	}

	SZEquipment->OnWardrobeUnquipped.AddUniqueDynamic(this, &ASZPlayerController::WardrobeUnequipped);
#pragma endregion

	// Possess 타이밍이 늦는 프로젝트도 있어 OnPossess에서 한 번 더 고정하면 안정적
	ApplyGameInputMode();
}

void ASZPlayerController::OnUnPossess()
{
#pragma region 델리게이트 수신 해제
	// 플레이어의 컴포넌트를 가져옴
	ASZCharacterPlayer* SZPlayer = Cast<ASZCharacterPlayer>(GetCharacter());
	if (!SZPlayer)
	{
		return;
	}

	USZInventoryBaseComponent* SZInventoryBase = SZPlayer->FindComponentByClass<USZInventoryBaseComponent>();
	if (!SZInventoryBase)
	{
		return;
	}

	// 옷장에서 아이템 장착
	SZInventoryBase->OnWardrobeEquipped.RemoveDynamic(this, &ASZPlayerController::WardrobeEquipped);

	// 옷장에서 아이템(장착템) 해제
	USZCharacterEquipmentComponent* SZEquipment = SZPlayer->GetEquipmentComponent();
	if (!SZEquipment)
	{
		return;
	}

	SZEquipment->OnWardrobeUnquipped.AddUniqueDynamic(this, &ASZPlayerController::WardrobeUnequipped);
#pragma endregion

	Super::OnUnPossess();
}

void ASZPlayerController::WardrobeEquipped(EEquipmentSlotType SlotType, USkeletalMesh* NewMesh)
{
	if (!IsValid(WardrobeActor))
	{
		return;
	}

	WardrobeActor->SetSKM(SlotType, NewMesh);
}

void ASZPlayerController::WardrobeUnequipped(EEquipmentSlotType SlotType)
{
	if (!IsValid(WardrobeActor))
	{
		return;
	}

	WardrobeActor->ClearSKM(SlotType);
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

void ASZPlayerController::TogglePauseMenu()
{
	const bool bIsPaused = IsPaused();

	if (!bIsPaused)
	{
		SetPause(true);
		ShowPauseMenu();
	}
	else
	{
		HidePauseMenu();
		SetPause(false);
	}
}

void ASZPlayerController::ShowPauseMenu()
{

	UE_LOG(LogTemp, Warning, TEXT("ShowPauseMenu called. PauseMenuClass=%s"),
		*GetNameSafe(PauseMenuClass));

	if (!PauseMenuClass) return;

	if (!PauseMenuWidget)
	{
		PauseMenuWidget = CreateWidget<UUserWidget>(this, PauseMenuClass);
	}
	if (PauseMenuWidget && !PauseMenuWidget->IsInViewport())
	{
		PauseMenuWidget->AddToViewport(100); // ZOrder 높게
	}

	bShowMouseCursor = true;
	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(PauseMenuWidget ? PauseMenuWidget->TakeWidget() : TSharedPtr<SWidget>());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}

void ASZPlayerController::HidePauseMenu()
{
	if (PauseMenuWidget && PauseMenuWidget->IsInViewport())
	{
		PauseMenuWidget->RemoveFromParent();
	}

	bShowMouseCursor = false;
	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void ASZPlayerController::ResumeFromPauseMenu()
{
	// 1) UI 제거
	if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;
	}

	// 2) 일시정지 해제
	SetPause(false);

	// 3) 마우스 커서/클릭/마우스오버 복구
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;

	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);

	// 4) 입력 모드 복구 (핵심)
	FInputModeGameOnly Mode;
	SetInputMode(Mode);

	// 5) (Enhanced Input에서 종종 필요) 키 상태 리셋
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RequestRebuildControlMappings(FModifyContextOptions(), EInputMappingRebuildType::Rebuild);
	}
}

void ASZPlayerController::ReturnToLobby()
{
	// 1) UI 제거
	if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;
	}

	// 2) 일시정지 해제 (OpenLevel 전에 반드시)
	SetPause(false);

	// 3) 마우스 커서/클릭/마우스오버 복구
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;

	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);

	// 4) 입력 모드 복구
	FInputModeGameOnly Mode;
	SetInputMode(Mode);

	// 5) (선택) Enhanced Input 키 상태/매핑 리빌드
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RequestRebuildControlMappings(FModifyContextOptions(), EInputMappingRebuildType::Rebuild);
	}

	// 6) 로비 레벨로 이동
	static const FName LobbyLevelName(TEXT("PAS_Demo1"));
	UGameplayStatics::OpenLevel(this, LobbyLevelName);
}

void ASZPlayerController::ExitGameFromPauseMenu()
{
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}
