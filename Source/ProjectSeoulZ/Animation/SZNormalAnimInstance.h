// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SZNormalAnimInstance.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 이혜성
// Date         : 2025-12-00
// Copyright    : CodingDead
//
// Description  : 일반 좀비 몬스터가 사용하는 애님 인스턴스.
//                
//                
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API USZNormalAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USZNormalAnimInstance();

protected:
	// 애님 인스턴스의 다양한 변수를 초기화하기 위한 함수.
	virtual void NativeInitializeAnimation() override;
	
	//
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 애님 인스턴스의 오너.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	// 캐릭터의 무브먼트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// 캐릭터의 속력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	// 캐릭터의 지상 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	// Idle인지 아닌지 확인하는 변수.
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = Character)
	uint8 bIsIdle : 1;

	// 이동상태인지 아닌지 확인하기 위한 변수
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Character)
	float MovingThreshould;
		
};
