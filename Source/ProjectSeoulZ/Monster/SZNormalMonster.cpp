// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/SZNormalMonster.h"
#include "SZNormalMonster.h"
#include "AI/SZNormalAIController.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Monster/GA/SZGA_NormalAttack.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Attribute/SZAttributeSet.h"
#include "UI/SZWidgetComponent.h"
#include "UI/SZUserWidget.h"
#include "UI/SZHpBarUserWidget.h"
#include <Kismet/GameplayStatics.h>
#include "GameMode/SZGameModeBase.h"
#include "GameMode/SZPoolManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/SZCharacterPlayer.h"

ASZNormalMonster::ASZNormalMonster()
{
	// 해당 클래스가 사용할 AIController 클래스를 지정한다.
	AIControllerClass = ASZNormalAIController::StaticClass();
	// Monster가 배치되었거나 Spawn되면 AIController가 빙의 됨.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	// 애님 인스턴스 할당
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Animation/ABP_NormalMonster.ABP_NormalMonster_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// SetDead에서 플레이할 DeadMontage 할당.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadAnimMontageRef(TEXT("/Game/Animation/Monster/AM_Dead.AM_Dead"));
	if (DeadAnimMontageRef.Object)
	{
		DeadMontage = DeadAnimMontageRef.Object;
	}

	// ASC와 AttributeSet 할당.
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<USZAttributeSet>(TEXT("AttributeSet"));


	/*HpBar = CreateDefaultSubobject<USZWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 8.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/HUD/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class) 
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::World);
		HpBar->SetDrawSize(FVector2D(20.0f, 3.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}*/

	/*HpBar = CreateDefaultSubobject<USZWidgetComponent>(TEXT("HPBarWidget"));
	HpBar->SetupAttachment(GetRootComponent());

	HpBar->SetWidgetSpace(EWidgetSpace::World);
	HpBar->SetDrawAtDesiredSize(true);
	HpBar->SetDrawSize(FVector2D(200.f, 40.f));
	HpBar->SetWorldScale3D(FVector(0.1f));
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	HpBar->SetPivot(FVector2D(0.5f, 0.f));
	HpBar->SetVisibility(true);
	HpBar->SetHiddenInGame(false);*/

	// HpBar를 SZWidgetComponent 클래스 CDO 생성.
	HpBar = CreateDefaultSubobject<USZWidgetComponent>(TEXT("HPBarWidget"));
	// HpBar를 메시에 부착.
	HpBar->SetupAttachment(GetMesh());

	//
	HpBar->SetWidgetSpace(EWidgetSpace::World);
	HpBar->SetDrawAtDesiredSize(true);
	HpBar->SetDrawSize(FVector2D(400.f, 20.f));
	HpBar->SetWorldScale3D(FVector(0.015f));
	HpBar->SetRelativeLocation(FVector(0, 0, 8.5));
	HpBar->SetPivot(FVector2D(0.5f, 0.f));
	HpBar->SetVisibility(true);
}


void ASZNormalMonster::SetDead()
{
	// 캐릭터의 움직을 막음
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	// 애님 인스턴스를 가져와 현재 재생중인 몽타주를 막는다.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);

	// DeadMontage를 재생
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
	
	// AIController를 가져와 AI를 멈춤.
	ASZNormalAIController* NormalMonsterController = Cast<ASZNormalAIController>(GetController());
	NormalMonsterController->StopAI();

	// 몽타주가 재생되는 시간동안 기다렸다가 오브젝트 풀에 반환하는 ReturnMonster함수 실행.
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&ASZNormalMonster::ReturnMonster,
		2.0f,
		false
	);

	// 게임 모드를 가져오기 위해 World 가져오기
	UWorld* World = GetWorld();

	// 게임 모드를 가져온다.
	ASZGameModeBase* GameMode = Cast<ASZGameModeBase>(World->GetAuthGameMode());
	// 가져온 게임 모드에서 살아있는 몬스터의 수를 나타내는 변수를 줄임
	GameMode->SetMonsterCount((GameMode->GetMonsterCount()) - 1);

}

UAbilitySystemComponent* ASZNormalMonster::GetAbilitySystemComponent() const
{
	return ASC;
}

void ASZNormalMonster::ReturnMonster()
{
	// 오브젝트 매니저를 담고 있는 게임 모드를 가져옴
	ASZGameModeBase* GameModeBase = Cast<ASZGameModeBase>(GetWorld()->GetAuthGameMode());
	// 가져온 게임 모드에서 오브젝트 매니저를 가뇨옴
	USZPoolManager* PoolManager = Cast<USZPoolManager>(GameModeBase->GetPoolManager());
	//
	PoolManager->ReturnActor(this);
}

// 밑의 네가지 함수들은 나중에 AttributeSet으로 교체가능성 있음.
float ASZNormalMonster::GetAIPatrolRadius()
{
	return 1500.0f;
}

float ASZNormalMonster::GetAIDetectRange()
{
	return 1000.0f;
}

float ASZNormalMonster::GetAIAttackRange()
{
	return 100.0f;
}

float ASZNormalMonster::GetAITurnSpeed()
{
	return 2.0f;
}

void ASZNormalMonster::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ASZNormalMonster::AttackByAI(class UBTTask_Attack* Task)
{
	//
	FGameplayAbilitySpec* AttackGASpec = ASC->FindAbilitySpecFromClass(USZGA_NormalAttack::StaticClass());
	if (!AttackGASpec)
	{
		UE_LOG(LogTemp, Log, TEXT("Error"));
		return; 
	}

	// 현재 공격 태스크에 전달 받은 태스크(BTTask_Attack) 몬스터 변수에 저장.
	CurrentAttackTask = Task;

	// 공격 어빌리티 실행.
	if (!AttackGASpec->IsActive())
	{
		ASC->TryActivateAbility(AttackGASpec->Handle);
	}

}

UAnimMontage* ASZNormalMonster::GetAttackAnimMontage()
{
	if (AttackDataAsset->AttackData.AttackMontage)
	{
		return AttackDataAsset->AttackData.AttackMontage;
	}
	else {
		return nullptr;
	}
}

int ASZNormalMonster::GetSectionCount()
{
	return AttackDataAsset->AttackData.SectionCount;
}


void ASZNormalMonster::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] ASC is nullptr!"), *GetName());
		return;
	}

	if (!NewController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Controller is nullptr! Cannot Init ASC."), *GetName());
		return;
	}

	if (!AttributeSet)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] AttributeSet is nullptr! Cannot Init ASC."), *GetName());
		return;
	}
	/*
	//
	if (ASC)
	{
		ASC->InitAbilityActorInfo(NewController, this);
	}

	//
	ASC->ApplyGameplayEffectToSelf(
		MonsterInitGE.GetDefaultObject(),
		1.f,
		ASC->MakeEffectContext()
	);

	//
	AttributeSet->InitHealth(AttributeSet->GetMaxHealth());
	*/
	//
	
	InitializeASC(NewController);
	FGameplayAbilitySpec AttackSkillSpec(USZGA_NormalAttack::StaticClass());
	ASC->GiveAbility(AttackSkillSpec);

}

void ASZNormalMonster::OnAttackHitNotify()
{
	//
	UE_LOG(LogTemp, Log, TEXT("OnAttackHitNotify Ok"));
	if (CurrentAttackAbility)
	{
		//
		ISZAttackAblilityInterface* Ability = Cast<ISZAttackAblilityInterface>(CurrentAttackAbility);
		if (Ability)
		{
			Ability->HitCheck();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("OnAttackHitNotify is nullptr"));
		}
	}
}

void ASZNormalMonster::OnSpawnFromPool_Implementation()
{
	// AI Run 시키기
	ASZNormalAIController* NormalController = Cast<ASZNormalAIController>(GetController());

	//
	NormalController->RunAI();
	
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	
	InitializeASC(GetController());

	//
	UE_LOG(LogTemp, Log, TEXT("Monster OnSpawnFromPool Call"));
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

}

void ASZNormalMonster::OnReturnToPool_Implementation()
{
	// AI Stop 시키기
	ASZNormalAIController* NormalController = Cast<ASZNormalAIController>(GetController());

	//
	NormalController->StopAI();

	//
	UE_LOG(LogTemp, Log, TEXT("Monster OnReturnFromPool Call"));
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ASZNormalMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//
	if (HpBar && HpBar->GetWidget())
	{
		//
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (CameraManager)
		{
			//
			FVector CameraLocation = CameraManager->GetCameraLocation();
			FVector HpBarLocation = HpBar->GetComponentLocation();

			// 카메라 방향 벡터
			FVector LookAtDir = CameraLocation - HpBarLocation;
			FRotator LookAtRot = LookAtDir.Rotation();

			//
			FRotator CurrentRot = HpBar->GetComponentRotation();
			FRotator TargetRot = LookAtDir.Rotation();
			TargetRot.Pitch = 0.f; TargetRot.Roll = 0.f;

			//
			FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 5.f); // 5.f = 회전 속도
			HpBar->SetWorldRotation(NewRot);
		}
	}
}



void ASZNormalMonster::HitByPlayer(const FGameplayEffectContextHandle& EffectContext)
{
	AActor* DamageCauser = EffectContext.GetEffectCauser();
	if (!DamageCauser) return;
	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(DamageCauser);
	/*if (Player)
	{
		FVector KnockbackDir = (GetActorLocation() - DamageCauser->GetActorLocation());
		KnockbackDir.Normalize();

		float KnockbackPower = 800.0f;

		FVector LaunchVelocity = KnockbackDir * KnockbackPower;

		LaunchCharacter(LaunchVelocity, true, true);
	}*/

	const FHitResult* HitResult = EffectContext.GetHitResult();

	FVector_NetQuantize HitLocation = HitResult
		? FVector_NetQuantize(HitResult->ImpactPoint)
		: FVector_NetQuantize(GetActorLocation());

	FVector_NetQuantizeNormal HitNormal = HitResult
		? FVector_NetQuantizeNormal(HitResult->ImpactNormal)
		: FVector_NetQuantizeNormal(
			(GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal()
		);

	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitEffect,
			HitLocation,
			HitNormal.Rotation()
		);
	}
}

void ASZNormalMonster::SetThePlayerForChase(const FGameplayEffectContextHandle& EffectContext)
{
	AActor* DamageCauser = EffectContext.GetEffectCauser();
	if (!DamageCauser) return;

	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(DamageCauser);
	AAIController* AI = Cast<AAIController>(GetController());
	if ( AI && Player)
	{
		UBlackboardComponent* BB = AI->GetBlackboardComponent();
		BB->SetValueAsBool("IsHit",true);
		BB->SetValueAsObject("Target", DamageCauser);
		
		UCharacterMovementComponent* MoveComp = this->GetCharacterMovement();
		if (MoveComp)
		{
			UE_LOG(LogTemp, Log, TEXT("SetThePlayerForChase"));
			MoveComp->MaxWalkSpeed = 300.f; // 예: 속도 변경
		}
	}
}

void ASZNormalMonster::InitializeASC(AController* NewController)
{
	UE_LOG(LogTemp,Log,TEXT("Initializing ASZNormalMonster"));
	if (ASC)
	{
		FGameplayTagContainer MonsterTags;
		MonsterTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Effect.Monster")));

		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(MonsterTags);
		ASC->CancelAllAbilities();
		ASC->RemoveActiveEffects(Query);

		// 2. ASC 초기화
		ASC->InitAbilityActorInfo(NewController, this);

		// 3. GE 생성 (SpecHandle)
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(MonsterInitGE, 1.f, EffectContext);
		float MaxHealth = AttributeSet->GetMaxHealth();
		
		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Health")), MaxHealth);
			// 4. Runtime 태그 추가 (Editor에서 Tags 안 보이는 경우)
			SpecHandle.Data->DynamicGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Effect.Monster")));

			// 5. GE 적용
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
		AttributeSet->InitHealth(AttributeSet->GetMaxHealth());
	}
}







