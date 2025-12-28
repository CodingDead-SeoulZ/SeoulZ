// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/SZInventoryBaseComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

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

