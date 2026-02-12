// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SZPortalActor.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2026-02-12
// Copyright    : 
//
// Description : 포탈 액터
//               캐릭터와 overlap 시 맵 이동
//				 
//                 
//----------------------------------------------------------------------------------------------------------

class USceneComponent;
class UStaticMeshComponent;
class UCapsuleComponent;

UCLASS()
class PROJECTSEOULZ_API ASZPortalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASZPortalActor();

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

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> Trigger;
};
