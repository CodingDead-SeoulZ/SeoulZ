// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/Inventory/SZInteractionUI.h"
#include "Item/Components/SZItemDataComp.h"
#include "SZInteractionComponent.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-17
// Copyright    : Coding Dead
//
// Description  : 아이템 상호작용 컴포넌트
//               충돌로 아이템을 감지하고, 아이템 정보 UI를 띄워주며 아이템을 줍는 기능을 담당
//             
//----------------------------------------------------------------------------------------------------------

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSEOULZ_API USZInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USZInteractionComponent();

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void BeginInteract();

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void UpdateInteract();

	USZInteractionUI* GetInteractionUI() const;
	void InteractionTrace();
	void PickUpItem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FTimerHandle InteractionTimerHandle;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TraceChannel")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

private:
	UPROPERTY()
	TObjectPtr<AActor> Item;

	FTimerHandle TimerHandle;
};
