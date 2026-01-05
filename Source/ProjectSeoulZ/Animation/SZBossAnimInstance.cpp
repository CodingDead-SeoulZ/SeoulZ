// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SZBossAnimInstance.h"
#include "SZBossAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USZBossAnimInstance::USZBossAnimInstance()
{
	MovingThreshould = 3.f;
}

void USZBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void USZBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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
