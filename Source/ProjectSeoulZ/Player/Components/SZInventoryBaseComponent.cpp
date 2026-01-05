// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/SZInventoryBaseComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Player/SZCharacterPlayer.h"

// Sets default values for this component's properties
USZInventoryBaseComponent::USZInventoryBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

const FItemTemplete* USZInventoryBaseComponent::FindItemData(FName ItemID) const
{
	// TObjectPtr는 GC가 추적하는 UObject 전용 포인터라서, USTRUCT 같은 일반 구조체에는 사용할 수 없음.
	// C++의 참조(T&)는 반드시 실제 객체를 가리켜야 하므로 nullptr이라는 개념이 없음.
	if (ItemData)
	{
		return ItemData->FindRow<FItemTemplete>(ItemID, TEXT("GetItemData"));
	}
	return nullptr;
}

int32 USZInventoryBaseComponent::FindMatchingSlot(FName ItemID) const
{
	const FItemTemplete* Item = FindItemData(ItemID);
	if (!Item) 
		return INDEX_NONE;

	for (int32 i = 0; i < ItemSlots.Num(); ++i)
	{
		const FItemSlot& Slot = ItemSlots[i];

		if (Slot.ItemID == ItemID &&
			Slot.StackCount > 0 &&
			Slot.StackCount <= Item->MaxStackCount)
		{
			return i;
		}
	}

	return INDEX_NONE;
}

int32 USZInventoryBaseComponent::FindEmptySlot() const
{
	for (int32 i = 0; i < ItemSlots.Num(); ++i)
	{
		if (ItemSlots[i].StackCount == 0)
		{
			return i;
		}
	}
	return -1;
}

void USZInventoryBaseComponent::AddToSlot(FName ItemID, int32 Index, int32 ItemCount)
{
	if (!ItemSlots.IsValidIndex(Index) || ItemCount <= 0)
	{
		return;
	}

	const FItemTemplete* Item = FindItemData(ItemID);
	if (!Item)
		return;

	// TODO. MaxStackCount에서 남은 아이템 처리 어떻게 할지
	ItemSlots[Index].StackCount += ItemCount;
}

void USZInventoryBaseComponent::AddToNewSlot(FName ItemID, int32 ItemCount, int32 Index)
{
	if (!ItemSlots.IsValidIndex(Index))
	{
		return;
	}

	ItemSlots[Index].ItemID = ItemID;
	ItemSlots[Index].StackCount = ItemCount;
}

void USZInventoryBaseComponent::PlayItemSFX(USoundBase* Sound) const
{
	if (Sound)
	{
		const AActor* OwnerActor = GetOwner();
		if (OwnerActor)
		{
			UGameplayStatics::PlaySoundAtLocation(this, Sound, OwnerActor->GetActorLocation());
		}
	}
}

const FItemSFX* USZInventoryBaseComponent::GetItemSFX(FName ItemID) const
{
	const FItemTemplete* Item = FindItemData(ItemID);
	return Item ? &Item->ItemSFX : nullptr;
}

int32 USZInventoryBaseComponent::PickUp(FName ItemID, int32 ItemCount)
{
	int32 LItemCount = FMath::Max(0, ItemCount);
	bool bLIsFull = false;

	while (LItemCount > 0 && !bLIsFull)
	{
		const int32 index = FindMatchingSlot(ItemID);
		if (index != -1 /*INDEX_NONE*/)
		{
			AddToSlot(ItemID, index, 1);
		}
		else
		{
			const int32 EmptyIndex = FindEmptySlot();
			if (EmptyIndex != -1 /*INDEX_NONE*/)
			{
				AddToNewSlot(ItemID, 1, EmptyIndex);
			}
			/*else
			{
				bLIsFull = true;
				return ItemSlots[index].StackCount;
			}*/
		}

		LItemCount -= 1;
		PlayItemSFX(GetItemSFX(ItemID)->Pickup);
	}

	return LItemCount;
}

void USZInventoryBaseComponent::UpdateInventory()
{
	OnInventoryUpdated.Broadcast();
}

void USZInventoryBaseComponent::TransferSlots(int32 Index, int32 SourceIndex, USZInventoryBaseComponent* SourceInventory)
{
	if (!ItemSlots.IsValidIndex(Index) ||
		!IsValid(SourceInventory) ||
		!SourceInventory->ItemSlots.IsValidIndex(SourceIndex))
	{
		return;
	}

	FItemSlot& DestinationSlot = ItemSlots[Index];
	FItemSlot& SourceSlot = SourceInventory->ItemSlots[SourceIndex];

	// 이동 불가
	if (SourceSlot.ItemID.IsNone() || SourceSlot.StackCount <= 0)
	{
		return;
	}

	const bool bSameItem = (DestinationSlot.ItemID == SourceSlot.ItemID) && !DestinationSlot.ItemID.IsNone();
	if (bSameItem)
	{
		// 동일한 아이템 -> 수량 합침 및 이동
		ItemTransfer(SourceInventory, DestinationSlot, SourceSlot, Index, SourceIndex);
	}
	else
	{
		// 다른 아이템 -> 스왑
		const FName  TempID = DestinationSlot.ItemID;
		const int32  TempCount = DestinationSlot.StackCount;

		AddToNewSlot(SourceSlot.ItemID, SourceSlot.StackCount, Index);
		SourceInventory->AddToNewSlot(TempID, TempCount, SourceIndex);
	}

	// TODO. SFX 사운드 넣기 
	if (const auto* Sfx = GetItemSFX(SourceSlot.ItemID))
	{
		if (Sfx->Move)
		{
			PlayItemSFX(Sfx->Move);
		}
	}

	// 브로드 캐스트
	UpdateInventory();
	if (SourceInventory != this)
	{
		SourceInventory->UpdateInventory();
	}
}

void USZInventoryBaseComponent::ItemTransfer(USZInventoryBaseComponent* SourceInventory,
	FItemSlot& DestinationSlot, FItemSlot& SourceSlot,
	int32 Index, int32 SourceIndex)
{
	if (!IsValid(SourceInventory))
	{
		return;
	}

	// 같은 아이템일 때만 호출
	if (DestinationSlot.ItemID != SourceSlot.ItemID || DestinationSlot.ItemID.IsNone())
	{
		return;
	}

	const FItemTemplete* Item = FindItemData(DestinationSlot.ItemID);
	if (!Item)
	{
		return;
	}

	const int32 MaxStack = Item->MaxStackCount;
	const int32 OriginalStack = DestinationSlot.StackCount;
	const int32 RemainingStack = MaxStack - OriginalStack;
	
	// 이미 꽉 참
	if (RemainingStack <= 0)
	{
		return; 
	}

	// 배치 가능
	const int32 SourceStackCount = SourceSlot.StackCount;
	const int32 CanMove = FMath::Min(RemainingStack, SourceStackCount);
	const int32 MergedStack = OriginalStack + CanMove;
	AddToNewSlot(DestinationSlot.ItemID, MergedStack, Index);

	// TODO. 빈 슬롯 규칙
	// 소스가 0이 되면 비우기
	const int32 RemainingSourceStack = SourceStackCount - CanMove;
	if (RemainingSourceStack <= 0)
	{
		AddToNewSlot(NAME_None, 0, SourceIndex);
	}
	else
	{
		SourceInventory->AddToNewSlot(SourceSlot.ItemID, RemainingSourceStack, SourceIndex);
	}
}

bool USZInventoryBaseComponent::CheckInventory(const USZInventoryBaseComponent* InInventory, const FItemSlot& InSourceSlot, int32& OutIndex, FItemSlot& OutItemSlot) const
{
	OutIndex = -1;
	OutItemSlot = FItemSlot{};

	if (!IsValid(InInventory) || InSourceSlot.ItemID.IsNone())
	{
		return false;
	}

	const FItemTemplete* Item = FindItemData(InSourceSlot.ItemID);
	if (!Item)
	{
		return false;
	}

	const int32 MaxStack = Item->MaxStackCount;
	for (int32 i = 0; i < InInventory->ItemSlots.Num(); ++i)
	{
		const FItemSlot& Slot = InInventory->ItemSlots[i];

		// 같은 ItemIDd의 아이템 중 아직 MaxStack에 도달하지 않은 슬롯 발견
		const bool bSameItem = (Slot.ItemID == InSourceSlot.ItemID);
		const bool bHasSpace = (Slot.StackCount < MaxStack);
		if (bSameItem && bHasSpace)
		{
			OutIndex = i;
			OutItemSlot = Slot;     
			return true;           
		}
	}

	return false;
}

void USZInventoryBaseComponent::MoveToInventory(USZInventoryBaseComponent* DestinationInventory, int32 SourceIndex)
{
	if (!IsValid(DestinationInventory) || !ItemSlots.IsValidIndex(SourceIndex))
	{
		return;
	}

	FItemSlot& SourceSlot = ItemSlots[SourceIndex];
	if (SourceSlot.ItemID.IsNone() || SourceSlot.StackCount <= 0)
	{
		return;
	}

	const FItemTemplete* Item = FindItemData(SourceSlot.ItemID);
	if (!Item)
	{
		return;
	}

#pragma region CheckInventory
	const int32 MaxStack = Item->MaxStackCount;

	int32 MatchingIndex = INDEX_NONE;
	FItemSlot MatchingSlot{};
	// 1) 목적지에 같은 아이템이면서 공간 있는 슬롯이 있는지 체크
	const bool bHasMatching = CheckInventory(DestinationInventory, SourceSlot, MatchingIndex, MatchingSlot);
	if (bHasMatching && DestinationInventory->ItemSlots.IsValidIndex(MatchingIndex))
	{
		const int32 Total = MatchingSlot.StackCount + SourceSlot.StackCount;
		if (Total > MaxStack)
		{
			// MaxStack까지 채움
			DestinationInventory->AddToNewSlot(SourceSlot.ItemID, MaxStack, MatchingIndex);
			
			// 남은 수량 처리
			const int32 Remainder = Total - MaxStack;
			const int32 EmptyIndex = DestinationInventory->FindEmptySlot();
			if (EmptyIndex != INDEX_NONE && EmptyIndex >= 0)
			{
				// 빈 슬롯에서 처리
				DestinationInventory->AddToNewSlot(SourceSlot.ItemID, Remainder, EmptyIndex);
				AddToNewSlot(NAME_None, 0, SourceIndex);
			}
			else
			{
				// 목적지에 더 넣을 공간이 없으니, 소스 슬롯에 남은 수량 유지
				SourceSlot.StackCount = Remainder;
			}
		}
		else
		{
			// 2-2) 목적지에 같은 아이템 슬롯이 없으면 빈 슬롯에 아이템 전부 넣기
			DestinationInventory->AddToNewSlot(SourceSlot.ItemID, Total, MatchingIndex);
			// 현재 슬롯 비우기
			AddToNewSlot(NAME_None, 0, SourceIndex);
		}
	}
	else
	{
		const int32 EmptyIndex = DestinationInventory->FindEmptySlot();
		if (EmptyIndex != INDEX_NONE && EmptyIndex >= 0)
		{
			// 3) 목적지에 같은 아이템 슬롯이 없으면 빈 슬롯에 아이템 전부 넣기
			DestinationInventory->AddToNewSlot(SourceSlot.ItemID, SourceSlot.StackCount, EmptyIndex);
			// 현재 슬롯 비우기
			AddToNewSlot(NAME_None, 0, SourceIndex);
		}
		else
		{
			// 빈 슬롯도 없으면
		}
	}

#pragma endregion

	if (const FItemSFX* Sfx = GetItemSFX(SourceSlot.ItemID))
	{
		if (Sfx->Move)
		{
			PlayItemSFX(Sfx->Move);
		}
	}

	UpdateInventory();
	if (DestinationInventory != this)
	{
		DestinationInventory->UpdateInventory();
	}
}

USkeletalMeshComponent* USZInventoryBaseComponent::GetPlayerPartBySlotType(ASZCharacterPlayer* Player, EEquipmentSlotType SlotType) const
{
	if (!Player) 
	{
		return nullptr;
	}

	switch (SlotType)
	{
	// 의상
	case EEquipmentSlotType::Helmet:     return Player->Helmet.Get();
	case EEquipmentSlotType::Vest:       return Player->Vest.Get();
	case EEquipmentSlotType::Gloves:     return Player->Gloves.Get();
	// 무기
	case EEquipmentSlotType::Holster:       return Player->Holster.Get();
	case EEquipmentSlotType::Magazine:      return Player->Magazine.Get();
	case EEquipmentSlotType::PrimaryGun:   return Player->PrimaryGun.Get();
	case EEquipmentSlotType::SecondaryGun: return Player->SecondaryGun.Get();
	default: return nullptr;
	}
}

bool USZInventoryBaseComponent::RequestUseItem(FName ItemID, int32 Index)
{
#pragma region 아이템 검증
	if (ItemID.IsNone() || !ItemSlots.IsValidIndex(Index))
	{
		return false;
	}

	// 슬롯에 해당 아이템이 있는지
	if ((ItemSlots[Index].ItemID != ItemID) || (ItemSlots[Index].StackCount <= 0))
	{
		return false;
	}

	const FItemTemplete* Item = FindItemData(ItemID);
	if (!Item)
	{
		return false;
	}
#pragma endregion

#pragma region 아이템 사용
	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(GetOwner());
	if (!Player)
	{
		return false;
	}

	const TSubclassOf<UGameplayEffect> GE = Item->ItemFragment.GE;
	const float Level = Item->ItemFragment.Level;

	// 카테고리 별
	switch (Item->ItemCategory)
	{
	case EItemCategory::Consumables:
	{
		const bool bIsValid = Player->ApplyInstantGE(GE, Level);
		if (!bIsValid)
		{
			return false;
		}
		break;
	}
	case EItemCategory::Appeal:
	{
		// TODO. WardrobeUI 및 EquipSlot 작업하기 -> 장비 해제 및 교체 
		const bool bEquip = EquipItem(ItemID, GE, Level);
		if (!bEquip) 
		{
			return false;
		}
		break;
	}
	case EItemCategory::Weapons:
	{
		
		//else if (bAccessory)
		//{
		//	// TODO. 
		//	StatText = FText::FromString(TEXT("공격력 없음"));
		//}
		break;
	}
	default:
		break;
	}

	ItemSlots[Index].StackCount = FMath::Max(0, ItemSlots[Index].StackCount - 1);
	// 0이면 아이템 제거
	if (ItemSlots[Index].StackCount == 0)
	{
		ItemSlots[Index].ItemID = NAME_None;
	}

	UpdateInventory();
	return true;
#pragma endregion
}

bool USZInventoryBaseComponent::RemoveEquippedItem(int32 Index, EEquipmentSlotType EquipmentSlot)
{
	if (!ItemSlots.IsValidIndex(Index))
	{
		return false;
	}

	const FItemSlot& Slot = ItemSlots[Index];
	if (Slot.ItemID.IsNone() || Slot.StackCount <= 0)
	{
		return false;
	}

	const FItemTemplete* Item = FindItemData(Slot.ItemID);
	if (!Item)
	{
		return false;
	}

	const bool bIsWeapon = (Item->ItemCategory == EItemCategory::Weapons);
	const bool bIsApparel = (Item->ItemCategory == EItemCategory::Appeal);
	const bool bEquip = (Item->Equipment.EquipmentSlotType == EquipmentSlot);
	const bool bCheck = ((bIsWeapon || bIsApparel) && (bEquip));
	if (!bCheck)
	{
		return false;
	}

	AddToNewSlot(NAME_None, 0, Index);
	UpdateInventory();
	return true;
}

bool USZInventoryBaseComponent::EquipPlayerCharacter(USkeletalMeshComponent* SkeletalComponent, EEquipmentSlotType EquipmentSlot, USkeletalMesh* NewMesh)
{
	if (!IsValid(SkeletalComponent) || !IsValid(NewMesh))
	{
		return false;
	}

	// 중복 방지
	if (SkeletalComponent->GetSkeletalMeshAsset() == NewMesh)
	{
		return false;
	}

	SkeletalComponent->SetSkeletalMesh(NewMesh);
	// 델리게이트
	OnWardrobeActorChanged.Broadcast(EquipmentSlot, NewMesh);
	return true;
}

bool USZInventoryBaseComponent::EquipItem(const FName InItemID, const TSubclassOf<UGameplayEffect>& GE, const float Level)
{
	if (!ItemData || InItemID.IsNone()) 
	{
		return false;
	}

	const FItemTemplete* Item = FindItemData(InItemID);
	if (!Item) 
	{
		return false;
	}

	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(GetOwner());
	if (!Player) 
	{
		return false;
	}

	const FActiveGameplayEffectHandle ActiveHandle = Player->ApplyInfiniteGE(GE, Level);
	if (!ActiveHandle.IsValid())
	{
		return false;
	}

	USkeletalMesh* NewMesh = Item->ItemMesh.SkeletalMesh;
	if (!IsValid(NewMesh)) 
	{
		return false;
	}

	const EEquipmentSlotType SlotType = Item->Equipment.EquipmentSlotType;
	USkeletalMeshComponent* TargetPart = GetPlayerPartBySlotType(Player, SlotType);
	if (!IsValid(TargetPart)) 
	{
		return false;
	}

	return EquipPlayerCharacter(TargetPart, SlotType, NewMesh);
}

void USZInventoryBaseComponent::PrintInventory()
{
	for (int32 i = 0; i < ItemSlots.Num(); ++i)
	{
		const FItemSlot Slot = ItemSlots[i];
		const FString ResultString = FString::Printf(
			TEXT("Slot %d | ItemID: %s | StackCount: %d"),
			i,
			*Slot.ItemID.ToString(),
			Slot.StackCount
		);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,ResultString);
		}
	}
}

// Called when the game starts
void USZInventoryBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemSlots.SetNum(MaxSlotCount);
}

// Called every frame
void USZInventoryBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}