// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/SZAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "Monster/SZNormalMonster.h"
#include "Player/SZCharacterPlayer.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffectExtension.h"

USZAttributeSet::USZAttributeSet() 
	/*AttackRange(100.0f),
	MaxAttackRange(300.0f),
	AttackRadius(50.0f),
	AttackRate(30.0f),
	MaxAttackRadius(150.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f),
	AttackDamage(30.0f),
	MaxAttackDamage(50.0f)*/
{
    InitMaxHealth(300.f);
    InitHealth(GetMaxHealth());
}

void USZAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    // 현재 체력 변수를 최대 체력 변수로 할당함.
    if (Attribute == GetHealthAttribute())
    {
    	NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }
}

void USZAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    AActor* OwnerActor = GetOwningActor();

    if (Attribute == GetHealthAttribute())
    {
        UE_LOG(LogTemp, Log, TEXT("Health : %f -> %f"), OldValue, NewValue);

        /*if (OldValue - NewValue > 0)
        {
            if (ASZNormalMonster* Monster = Cast<ASZNormalMonster>(OwnerActor))
            {
                USZNormalMonsterASC* ASC =
                    Cast<USZNormalMonsterASC>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor));

                if (ASC && ASC->LastDamageInstigator.IsValid())
                {
                    Monster->HitByPlayer(ASC->LastDamageInstigator.Get());
                    Monster->SetThePlayerForChase(ASC->LastDamageInstigator.Get());
                }
          
            }
        }*/
        // 데미지를 받은 이후에 체력이 0보다 작거나 같으면
        if (OldValue > 0.0f && NewValue <= 0.0f)
        {
            if (!OwnerActor || !OwnerActor->HasAuthority()) return; // 서버 체크

            // 플레이어인지 몬스터인지 구분
            if (ASZCharacterBase* Player = Cast<ASZCharacterBase>(OwnerActor))
            {
                // 플레이어의 SetDead() 함수 호출
                Player->SetDead();
            }
            else if (ASZNormalMonster* Monster = Cast<ASZNormalMonster>(OwnerActor))
            {
                // 몬스터의 SetDead() 함수 호출
                Monster->SetDead();
            }
        }
    }
}

void USZAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        ASZNormalMonster* Monster = Cast<ASZNormalMonster>(TargetActor);

        if (Monster)
        {
            Monster->HitByPlayer(Data.EffectSpec.GetEffectContext());
            Monster->SetThePlayerForChase(Data.EffectSpec.GetEffectContext());
        }

        const FGameplayEffectContextHandle& Context =Data.EffectSpec.GetEffectContext();
        const FHitResult* HitResult = Context.GetHitResult();
    }
}

//void USZAttributeSet::OnGEAppliedToSelf(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
//{
//    AActor* Instigator = Spec.GetEffectContext().GetInstigator();
//
//    if (Instigator)
//    {
//        LastDamageInstigator = Instigator;
//    }
//}

