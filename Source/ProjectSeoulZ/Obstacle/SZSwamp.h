// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameMode/SZPoolManager.h"
#include "Interface/SZPoolableInterface.h"
#include "SZSwamp.generated.h"

UCLASS()
class PROJECTSEOULZ_API ASZSwamp : public AActor, public IAbilitySystemInterface, public ISZPoolableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASZSwamp();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(VisibleAnywhere,Category=GAS)
	TSubclassOf<class UGameplayEffect> GameplayEffectClass;
	
protected:
	virtual void PostInitializeComponents() override;
	
	void ApplyEffectToTarget(AActor* Target);
	
protected:
	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;
	
	UPROPERTY(VisibleAnywhere,Category=Box)
	TObjectPtr<class UBoxComponent> Trigger;
	
	UPROPERTY(VisibleAnywhere,Category=Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;
	
	//Object Pooling Section
protected:
	virtual void OnSpawnFromPool_Implementation() override;

	//
	virtual void OnReturnToPool_Implementation() override;
	
};
