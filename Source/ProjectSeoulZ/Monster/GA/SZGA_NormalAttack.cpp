// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GA/SZGA_NormalAttack.h"
#include "SZGA_NormalAttack.h"
#include "Monster/SZNormalMonster.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/SZNormalAnimInstance.h"
#include "AI/BTTask_Attack.h"
#include "Physics/SZCollision.h"
#include "CollisionQueryParams.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"
#include "Attribute/SZAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"

USZGA_NormalAttack::USZGA_NormalAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USZGA_NormalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// NormalMonster ÇüÀ¸·Î ActorInfo¸¦ Ä³½ºÆÃ
	ASZNormalMonster* NormalMonster = CastChecked<ASZNormalMonster>(ActorInfo->AvatarActor.Get());

	if (NormalMonster)
	{
		NormalMonster->SetCurrentAttackAbility(this);
	}


	// °ø°Ý ¸ùÅ¸ÁÖ¿¡ ´ëÇÑ Á¤º¸¸¦ °¡Á®¿À±â À§ÇÑ Çüº¯È¯ ÈÄ nullptr Ã¼Å©
	ISZNormalAIInterface* Monster = Cast<ISZNormalAIInterface>(NormalMonster);
	if (Monster == nullptr)
	{
		return ;
	}

	// ÀÎÅÍÆäÀÌ½º¸¦ ÀÌ¿ëÇØ¼­ °ø°Ý ¸ùÅ¸ÁÖ¸¦ °¡Á®¿È. ¸ó½ºÅÍ¿¡ µû¶ó ´Þ¶óÁü.
	UAnimMontage* AttackMontage = Monster->GetAttackAnimMontage();
	if (AttackMontage == nullptr)
	{
		return;
	}

	// 1ºÎÅÍ ¸ùÅ¸ÁÖÀÇ ¼½¼Ç ¼ö±îÁö Áß ÇÏ³ª¸¦ 
	int SectionNumber = FMath::RandRange(1, Monster->GetSectionCount());
	UE_LOG(LogTemp, Log, TEXT("%d"),SectionNumber);

	// ÇÃ·¹ÀÌÇÒ ¼½¼Ç ÀÌ¸§ »ý¼º.
	FName SectionName = FName(*FString::Printf(TEXT("Attack%d"), SectionNumber));

	/*UE_LOG(LogTemp, Warning, TEXT("Avatar: %s"), *GetNameSafe(GetAvatarActorFromActorInfo()));

	USkeletalMeshComponent* Mesh =
		GetAvatarActorFromActorInfo()->FindComponentByClass<USkeletalMeshComponent>();

	UE_LOG(LogTemp, Warning, TEXT("Mesh: %s"), *GetNameSafe(Mesh));
	UE_LOG(LogTemp, Warning, TEXT("AnimInstance: %s"), *GetNameSafe(Mesh ? Mesh->GetAnimInstance() : nullptr));*/

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), AttackMontage,1.0f, SectionName);
	PlayAttackTask->OnCompleted.AddDynamic(this, &USZGA_NormalAttack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &USZGA_NormalAttack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();
}

void USZGA_NormalAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void USZGA_NormalAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ASZNormalMonster* NormalMonster = CastChecked<ASZNormalMonster>(ActorInfo->AvatarActor.Get());

	if (NormalMonster)
	{
		NormalMonster->SetCurrentAttackAbility(nullptr);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}



void USZGA_NormalAttack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;

	if (ASZNormalMonster* Monster = Cast<ASZNormalMonster>(GetAvatarActorFromActorInfo()))
	{
		if (Monster->GetAttackTask())
		{
			Monster->GetAttackTask()->OnAttackFinished(true);
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo,CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void USZGA_NormalAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void USZGA_NormalAttack::HitCheck()
{
	UE_LOG(LogTemp, Log, TEXT("HitCheck Ok"));

	ASZNormalMonster* NormalMonster = CastChecked<ASZNormalMonster>(GetAvatarActorFromActorInfo());

	FHitResult OutHitResult;
	FCollisionQueryParams Params(NAME_None, false, NormalMonster);

	UAbilitySystemComponent* ASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(NormalMonster);
	if (!ASC)
	{
		UE_LOG(LogTemp, Log, TEXT("ASC is nullptr"));
		return;
	}

	const USZAttributeSet* AttributeSet = ASC->GetSet<USZAttributeSet>();
	if (!AttributeSet)
	{
		UE_LOG(LogTemp, Log, TEXT("AttributeSet is null"));
		return;
	}

	// ³ªÁß¿¡ °ªÀ¸·Î ¾îÆ®¸®ºäÆ® ¼ÂÀ¸·Î ±³Ã¼ÇØ¾ß ÇÔ.
	const float AttackRange = AttributeSet->GetAttackRange();
	const float AttackRadius = AttributeSet->GetAttackRadius();
	const float AttackDamage = AttributeSet->GetAttackDamage();
	const FVector Start = NormalMonster->GetActorLocation() + NormalMonster->GetActorForwardVector() 
						  * NormalMonster ->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + NormalMonster->GetActorForwardVector() * AttackRange;

	// ÈÄ¿¡ ÇÃ·¹ÀÌ¾î Àü¿ë Ã¤³Î·Î ¹Ù²ã¾ß ÇÔ.
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_SZACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Ensured();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OutHitResult.GetActor());
		if (!SourceASC || !TargetASC)
		{
			UE_LOG(LogTemp, Log, TEXT("ASC not found!"));
			return;
		}

		const USZAttributeSet* SourceAttribute = SourceASC->GetSet<USZAttributeSet>();
		USZAttributeSet* TargetAttribute = const_cast<USZAttributeSet*>(TargetASC->GetSet<USZAttributeSet>());
		if (!SourceAttribute || !TargetAttribute)
		{
			UE_LOG(LogTemp, Log, TEXT("ASC not found!"));
			return;
			// í˜„ìž¬ ì²´ë ¥
			float Health = TargetAttribute->GetHealth();
			// í˜„ìž¬ ë°©ì–´ë ¥
			float Defense = TargetAttribute->GetDefense();
			// ë°©ì–´ë ¥ì´ ë‚®ì„ ê²½ìš° ë°©ì–´ë ¥ì´ í¬ê²Œ ì¦ê°€. ë°©ì–´ë ¥ì´ ë†’ì„ ê²½ìš° ë°©ì–´ë ¥ì´ ë‚®ê²Œ ì¦ê°€.
			Defense = Defense / (1 + Defense);
			// ìµœì¢… ë°ë¯¸ì§€ ê³„ì‚°
			float Damage = AttackDamage - Defense * 2; // Defense * 0.5
			TargetAttribute->SetHealth(Health - Damage);
		}

		TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);

	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(NormalMonster->GetActorForwardVector()).ToQuat(),DrawColor, false, 0.5f);


#endif

	
}


