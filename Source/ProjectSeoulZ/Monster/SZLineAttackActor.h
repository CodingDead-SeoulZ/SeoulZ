// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Interface/SZPoolableInterface.h"
#include "SZLineAttackActor.generated.h"

UCLASS(Blueprintable)
class PROJECTSEOULZ_API ASZLineAttackActor : public AActor,  public ISZPoolableInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()
	

public:
    ASZLineAttackActor();

	// 플레이어에게 데미지를 주기 위한 ASC를 위한 함수 
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// 오버랩이 시작되면 호출되는 함수.
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
	// 오버랩이 끝나면 호출되는 함수.
	UFUNCTION()
	void OnEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	// 데미지를 위한 GameplayEffects
	UPROPERTY(VisibleAnywhere,Category = GAS)
	TSubclassOf<class UGameplayEffect> GameplayEffectClass;

	// 블루프린트에서 호출할 OnSpawnFromPoolBlueprint. 블루프린트의 Timeline을 써야하기 때문에 BlueprintNativeEvent 사용
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSpawnFromPoolBlueprint();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ratio)
	float Progress;

protected:
	virtual void PostInitializeComponents() override;

	// 데미지를 주는 함수
	void ApplyEffectToPlayer(AActor* Target);

protected:
	// 오브젝트 풀에서 가져오고 사용할 때를 위한 함수
	virtual void OnSpawnFromPool_Implementation() override;

	// 오브젝트 풀에 반납할 때 필요한 함수
	virtual void OnReturnToPool_Implementation() override;


protected:
	// ASC
	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;

	// Collision 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess="true"),Category = "BPCollision")
	class UBoxComponent* Hit;

	// 이펙트를 끄고 켜기 위한 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "BPCollision")
	TObjectPtr<class UNiagaraComponent> NiagaraEffect;

	UPROPERTY()
	TArray<AActor*> OverlappingActors;

	FTimerHandle DamageTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageInterval = 0.1f;

	float Length = 251.6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StartPoint)
	TObjectPtr<class USceneComponent> ReferencePoint;

	void CheckDamage();

};
