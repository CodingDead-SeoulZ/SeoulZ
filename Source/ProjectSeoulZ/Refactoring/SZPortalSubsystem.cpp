// Fill out your copyright notice in the Description page of Project Settings.


#include "Refactoring/SZPortalSubsystem.h"
#include "Kismet/GameplayStatics.h"

void USZPortalSubsystem::RequestTravel(UObject* WorldContext)
{
	if (!WorldContext) { return; }

	UGameplayStatics::OpenLevel(WorldContext, MapName);
}
