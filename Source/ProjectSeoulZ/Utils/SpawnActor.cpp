
#include "SpawnActor.h"

AActor* FSpawnActors::SpawnActor(UWorld* World, TSubclassOf<AActor> ActorClass, const FVector& Location, const FRotator& Rotation)
{
	if (!World || !ActorClass)
		return nullptr;


	return  World->SpawnActor<AActor>(ActorClass, Location, Rotation);
}
