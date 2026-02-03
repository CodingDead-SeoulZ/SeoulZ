// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SZNormalAnimInstance.h"
#include "SZNormalAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USZNormalAnimInstance::USZNormalAnimInstance()
{
	// 이 값을 넘으면 Idle이 아닌 Move로 간주
	MovingThreshould = 3.0f;
}

// 다양한 변수 초기화.
void USZNormalAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

// 다양한 변수를 Tick마다 확인해 할당.
void USZNormalAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
	}
}
