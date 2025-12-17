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

USZGA_NormalAttack::USZGA_NormalAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USZGA_NormalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// NormalMonster 형으로 ActorInfo를 캐스팅
	ASZNormalMonster* NormalMonster = CastChecked<ASZNormalMonster>(ActorInfo->AvatarActor.Get());

	if (NormalMonster)
	{
		NormalMonster->SetCurrentAttackAbility(this);
	}


	// 공격 몽타주에 대한 정보를 가져오기 위한 형변환 후 nullptr 체크
	ISZNormalAIInterface* Monster = Cast<ISZNormalAIInterface>(NormalMonster);
	if (Monster == nullptr)
	{
		return ;
	}

	// 인터페이스를 이용해서 공격 몽타주를 가져옴. 몬스터에 따라 달라짐.
	UAnimMontage* AttackMontage = Monster->GetAttackAnimMontage();
	if (AttackMontage == nullptr)
	{
		return;
	}

	// 1부터 몽타주의 섹션 수까지 중 하나를 
	int SectionNumber = FMath::RandRange(1, Monster->GetSectionCount());
	UE_LOG(LogTemp, Log, TEXT("%d"),SectionNumber);

	// 플레이할 섹션 이름 생성.
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

//	ASZNormalMonster* NormalMonster = CastChecked<ASZNormalMonster>(GetAvatarActorFromActorInfo());
//
//	FHitResult OutHitResult;
//	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
//
//	// 나중에 값으로 어트리뷰트 셋으로 교체해야 함.
//	const float AttackRange = 40.0f;
//	const float AttackRadius = 50.0f;
//	const float AttackDamage = 30.0f;
//	const FVector Start = NormalMonster->GetActorLocation() + NormalMonster->GetActorForwardVector() 
//						  * NormalMonster ->GetCapsuleComponent()->GetScaledCapsuleRadius();
//	const FVector End = Start + NormalMonster->GetActorForwardVector() * AttackRange;
//
//	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_SZACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
//	if (HitDetected)
//	{
//
//	}
//
//#if ENABLE_DRAW_DEBUG
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	float CapsuleHalfHeight = AttackRange * 0.5;
//	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//
//	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(NormalMonster->GetActorForwardVector()).ToQuat(),DrawColor, false, 0.5f);
//
//
//#endif

	
}


