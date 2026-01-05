
#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

class FSpawnActors
{
public:
    static AActor* SpawnActor(
        UWorld* World,
        TSubclassOf<AActor> ActorClass,
        const FVector& Location,
        const FRotator& Rotation
    );
};




