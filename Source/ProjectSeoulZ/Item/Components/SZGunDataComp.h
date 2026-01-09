// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SZGunDataComp.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSEOULZ_API USZGunDataComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USZGunDataComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 아이템 아이디
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, Meta = (AllowPrivateAccess = "true"))
	FName ItemID = NAME_None;

	// 현재 장전된 탄약
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, Meta = (AllowPrivateAccess = "true"))
	int32 CurrentAmmo = 0;

	// 최대 장전 가능 탄약
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, Meta = (AllowPrivateAccess = "true"))
	int32 MaxAmmo = 12;

	// 인벤토리에 보유한 총알 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, Meta = (AllowPrivateAccess = "true"))
	int32 InventoryAmmo = 60;	
};
