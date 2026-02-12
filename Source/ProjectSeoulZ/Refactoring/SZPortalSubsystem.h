// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SZPortalSubsystem.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2026-02-12
// Copyright    : 
//
// Description : 포탈 서브시스템
//               싱글 게임에서 맵 이동 시 인벤토리 데이터를 초기화하기 위함.
//				 GameInstance가 아닌, GameInstanceSubsystem 사용.
// 
//  - UGameInstanceSubsystem은 GameInstance와 같은 수명 주기를 갖는 서브시스템
//  - 직접 GameInstance를 상속해 무거운 관리 코드를 넣을 필요 없이 모듈 단위로 기능을 분리할 수 있도록 설계된 구조
//				 
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API USZPortalSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void RequestTravel(UObject* WorldContext);

private:
	const FName MapName = "TestMap2";
};
