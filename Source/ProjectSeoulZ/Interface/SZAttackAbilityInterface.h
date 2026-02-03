
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SZAttackAbilityInterface.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 이혜성
// Date         : 2025-12-00
// Copyright    : CodingDead
//
// Description  : 몬스터가 사용하는 어빌리티가 호출하는 인터페이스. 
//                어빌리티마다 공격판정을 하는 HitCheck를 구현해야 한다.
//                
//                
//----------------------------------------------------------------------------------------------------------

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USZAttackAblilityInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTSEOULZ_API ISZAttackAblilityInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//
	virtual void HitCheck() = 0;
};
