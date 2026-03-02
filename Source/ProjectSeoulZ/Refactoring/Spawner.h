// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2026-03-02
// Copyright    : 
//
// Description : 아이템 스폰 액터
//               캐릭터와 overlap 시 아이템을 랜덤으로 스폰하는 역할
//				 
//                 
//----------------------------------------------------------------------------------------------------------

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class PROJECTSEOULZ_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UDataTable* ItemDataTable = nullptr;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Trigger;
};
