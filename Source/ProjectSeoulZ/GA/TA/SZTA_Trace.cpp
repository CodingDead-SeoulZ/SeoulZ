// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/SZTA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "Monster/SZNormalMonster.h"

ASZTA_Trace::ASZTA_Trace()
{
}

void ASZTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ASZTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle ASZTA_Trace::MakeTargetData() const
{
	/*ASZNormalMonster* Monster = CastChecked<ASZNormalMonster>(SourceActor);

	FHitResult OutHitResult;

	const float AttackRange = 100.0f;
	const float AttackRadius = 50.f;

	FCollisionQueryParams 

	return FGameplayAbilityTargetDataHandle();*/
	return nullptr;
}
