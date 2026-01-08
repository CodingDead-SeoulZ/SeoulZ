// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Interface/SZPoolableInterface.h"
#include "SZLineAttackActor.generated.h"

UCLASS()
class PROJECTSEOULZ_API ASZLineAttackActor : public AActor,  public ISZPoolableInterface/*public IAbilitySystemInterface*/
{
	GENERATED_BODY()
	

public:
    ASZLineAttackActor();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<class UBoxComponent> CollisionBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<class UNiagaraComponent> NiagaraComp;

};
