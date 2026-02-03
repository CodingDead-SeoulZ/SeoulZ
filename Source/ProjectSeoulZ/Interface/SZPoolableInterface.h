// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SZPoolableInterface.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 이혜성
// Date         : 2025-12-00
// Copyright    : CodingDead
//
// Description  : 오브젝트 풀링을 이용하는 액터는 구현해야하는 인터페이스
//				  OnSpawnFromPool은 풀에서 액터를 가져오는 기능을, 
//                OnReturnToPool은 풀로 액터를 반환하는 기능을 나타낸다.
//                
//                
//----------------------------------------------------------------------------------------------------------

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USZPoolableInterface : public UInterface
{
	GENERATED_BODY()
};


class PROJECTSEOULZ_API ISZPoolableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSpawnFromPool();

	//
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnReturnToPool();
};
