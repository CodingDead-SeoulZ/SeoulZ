// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SZMonsterAttackInterface.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 
// Date         : 2025-12-00
// Copyright    : 
//
// Description  : 
//                
//                
//----------------------------------------------------------------------------------------------------------

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USZMonsterAttackInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTSEOULZ_API ISZMonsterAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//
	virtual void OnAttackHitNotify() = 0;
};
