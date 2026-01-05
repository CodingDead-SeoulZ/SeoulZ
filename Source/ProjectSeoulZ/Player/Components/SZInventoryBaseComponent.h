// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/SZItemTemplete.h"
#include "SZInventoryBaseComponent.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-17
// Copyright    : Coding Dead
//
// Description  : 인벤토리 베이스 컴포넌트
//           아이템 데이터, 슬롯, 로직 등 인벤토리 기능. 보여주는 기능은 Actor에서 관리    
//             
//----------------------------------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StackCount = 0; 
};

class ASZCharacterPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWardrobeActorChanged, EEquipmentSlotType, SlotType, USkeletalMesh*, NewMesh);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSEOULZ_API USZInventoryBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USZInventoryBaseComponent();

#pragma region 아이템 줍기
	// 데이터 테이블
	const FItemTemplete* FindItemData(FName ItemID) const;
	// 슬롯
	int32 FindMatchingSlot(FName ItemID) const;
	int32 FindEmptySlot() const;
	void AddToSlot(FName ItemID, int32 Index, int32 ItemCount);
	void AddToNewSlot(FName ItemID, int32 ItemCount, int32 Index);
	// 사운드
	void PlayItemSFX(USoundBase* Sound) const;
	const FItemSFX* GetItemSFX(FName ItemID) const;
	// 아이템 줍기
	int32 PickUp(FName ItemID, int32 ItemCount);
#pragma endregion

#pragma region 아이템 이동
	void UpdateInventory();

	// 슬롯 이동
	void TransferSlots(int32 index, int32 SourceIndex, USZInventoryBaseComponent* SourceInventory);
	// 같은 아이템 수량 계산
	void ItemTransfer(USZInventoryBaseComponent* SourceInventory, FItemSlot& DestinationSlot, FItemSlot& SourceSlot,int32 Index, int32 SourceIndex);
#pragma endregion

#pragma region 아이템 상세보기 - 퀵 슬롯
	bool CheckInventory(
		const USZInventoryBaseComponent* InInventory, const FItemSlot& InSourceSlot,
		int32& OutIndex, FItemSlot& OutItemSlot
	) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|QuickSlot")
	void MoveToInventory(USZInventoryBaseComponent* DestinationInventory, int32 SourceIndex);
#pragma endregion

#pragma region 아이템 상세보기 - 아이템 사용
	USkeletalMeshComponent* GetPlayerPartBySlotType(ASZCharacterPlayer* Player, EEquipmentSlotType SlotType) const;
	bool RequestUseItem(FName ItemID, int32 InIndex);
	bool RemoveEquippedItem(int32 Index, EEquipmentSlotType EquipmentSlot);
	bool EquipPlayerCharacter(USkeletalMeshComponent* SkeletalComponent, EEquipmentSlotType EquipmentSlot, USkeletalMesh* NewMesh);
	bool EquipItem(const FName InItemID);
	void UseItem();
#pragma endregion

	UFUNCTION(BlueprintCallable)
	void PrintInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWardrobeActorChanged OnWardrobeActorChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory | Data")
	TObjectPtr<UDataTable> ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory | Inventory Count")
	int32 MaxSlotCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FItemSlot> ItemSlots;
};
