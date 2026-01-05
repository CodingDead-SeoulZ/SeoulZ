// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/SZBossBanshee.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Attribute/SZAttributeSet.h"
#include "AI/SZBossAIController.h"
#include "Monster/GA/SZGA_BansheeMonsterSpawn.h"

ASZBossBanshee::ASZBossBanshee()
{
	// �ش� Ŭ������ ����� AIController Ŭ������ �����Ѵ�.
	AIControllerClass = ASZBossAIController::StaticClass();
	// Monster�� ��ġ�Ǿ��ų� Spawn�Ǹ� AIController�� ���� ��.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetCharacterMovement()->bOrientRotationToMovement = false; // 루트 모션과 충돌 방지
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// �ִ� �ν��Ͻ� �Ҵ�
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Animation/Monster/ABP_BossBanshee.ABP_BossBanshee_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadAnimMontageRef(TEXT("/Game/Animation/Monster/AM_BossDead.AM_BossDead"));
	if (DeadAnimMontageRef.Object)
	{
		DeadMontage = DeadAnimMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RoarAnimMontageRef(TEXT("/Game/Animation/Monster/AM_Roar.AM_Roar"));
	if (RoarAnimMontageRef.Object)
	{
		RoarMontage = RoarAnimMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SpawnSkillMontageRef(TEXT("/Game/Animation/Monster/AM_SpawnUndead.AM_SpawnUndead"));
	if (SpawnSkillMontageRef.Object)
	{
		SpawnSkillMontage = SpawnSkillMontageRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SpawnSwampMontageRef(TEXT("/Game/Animation/Monster/AM_SpawnSwamp.AM_SpawnSwamp"));
	if (SpawnSwampMontageRef.Object)
	{
		SpawnSwampMontage = SpawnSwampMontageRef.Object;
	}

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<USZAttributeSet>(TEXT("AttributeSet"));
}

void ASZBossBanshee::BeginPlay()
{
	Super::BeginPlay();

	//PlayIntroMontage();
}

void ASZBossBanshee::SetDead()
{
	// ĳ������ ������ ����
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	// �ִ� �ν��Ͻ��� ������ ���� ������� ��Ÿ�ָ� ���´�.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);

	// DeadMontage�� ���
	AnimInstance->Montage_Play(DeadMontage, 1.0f);

	// AIController�� ������ AI�� ����. ���� AIController�� �ٲ�� ��.
	ASZBossAIController* NormalMonsterController = Cast<ASZBossAIController>(GetController());
	NormalMonsterController->StopAI();

	// ��Ÿ�ְ� ����Ǵ� �ð����� ��ٷȴٰ� ���͸� �ı��ϴ� �Լ� ȣ��.
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&ASZBossBanshee::DestroyFunc,
		2.0f,
		false
	);

	

}

UAbilitySystemComponent* ASZBossBanshee::GetAbilitySystemComponent() const
{
	return ASC;
}

// Ÿ�̸� ȣ���� ���� ���� �Լ�
void ASZBossBanshee::DestroyFunc()
{
	Destroy();
}

float ASZBossBanshee::GetAIPatrolRadius()
{
	return 1500.0f;
}

float ASZBossBanshee::GetAIDetectRange()
{
	return 500.0f;
}

float ASZBossBanshee::GetAIAttackRange()
{
	return 0.0f;
}

float ASZBossBanshee::GetAITurnSpeed()
{
	return 0.0f;
}

void ASZBossBanshee::PossessedBy(AController* NewController)
{
	if (ASC)
	{
		ASC->InitAbilityActorInfo(NewController, this);
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
			UE_LOG(LogTemp, Warning, TEXT("Start Ability: %s"), *StartAbility->GetName());
		}
	
		//
		ASC->ApplyGameplayEffectToSelf(
			BossInitGE.GetDefaultObject(),
			1.f,
			ASC->MakeEffectContext()
		);

		//
		AttributeSet->InitHealth(AttributeSet->GetMaxHealth());
		UE_LOG(LogTemp, Log, TEXT("PossessedBy End"));
	}

	//
	/*FGameplayAbilitySpec AttackSkillSpec(USZGA_NormalAttack::StaticClass());
	ASC->GiveAbility(AttackSkillSpec);*/
}

void ASZBossBanshee::PlayIntroMontage()
{
	UE_LOG(LogTemp, Log, TEXT("PlayIntroMontage Start"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);

	// DeadMontage�� ���
	AnimInstance->Montage_Play(RoarMontage, 1.0f);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&ASZBossBanshee::FinishRoar,
		4.14f,
		false
	);
	
}

void ASZBossBanshee::FinishRoar()
{
	UE_LOG(LogTemp, Log, TEXT("PlayIntroMontage End"));
}
