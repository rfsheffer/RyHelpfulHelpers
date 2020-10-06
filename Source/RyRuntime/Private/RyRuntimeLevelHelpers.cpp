// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeLevelHelpers.h"
#include "RyRuntimeModule.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/PackageName.h"
#include "Engine/Level.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ULevel* URyRuntimeLevelHelpers::GetActorLevel(const AActor* actorIn)
{
    if(!actorIn)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("URyRuntimeLevelHelpers::GetActorLevel called with invalid actorIn!"));
        return nullptr;
    }
    if(!actorIn->IsValidLowLevelFast(true) || actorIn->IsPendingKill())
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("URyRuntimeLevelHelpers::GetActorLevel called with actorIn that is pending kill!"));
        return nullptr;
    }

    return actorIn->GetTypedOuter<ULevel>();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeLevelHelpers::IsActorInLevel(AActor* actorToCheck, ULevel* levelToCheck)
{
    if(actorToCheck == nullptr || levelToCheck == nullptr)
        return false;

    return GetActorLevel(actorToCheck) == levelToCheck;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeLevelHelpers::GetLevelNameString(ULevel* levelIn, bool longName)
{
    if(levelIn == nullptr || levelIn->GetOutermost() == nullptr)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("URyRuntimeLevelHelpers::GetLevelNameString called with invalid levelIn!"));
        return TEXT("");
    }
    if(longName)
    {
        return levelIn->GetOutermost()->GetFName().ToString();
    }
    
    return FPackageName::GetShortName(levelIn->GetOutermost()->GetFName());
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeLevelHelpers::IsLevelPersistentLevel(ULevel* levelIn)
{
    if(levelIn == nullptr || levelIn->OwningWorld == nullptr)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("URyRuntimeLevelHelpers::IsLevelPersistentLevel called with invalid levelIn!"));
        return false;
    }

    return levelIn == levelIn->OwningWorld->PersistentLevel;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UObject* URyRuntimeLevelHelpers::FindObjectInLevelByName(ULevel* levelToSearch, const FString& nameToFind)
{
    return StaticFindObject(/*Class=*/ NULL, levelToSearch, *nameToFind, true);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeLevelHelpers::GetActorsOfTypeInLevel(ULevel* level, TSubclassOf<AActor> ActorClass, TArray<AActor*>& actorsOut)
{
    if(!level)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("RyRuntimeLevelHelpers::GetActorsOfTypeInLevel error. level is NULL!"));
        return;
    }

    UWorld* const World = level->GetWorld();
    if(!World)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("RyRuntimeLevelHelpers::GetActorsOfTypeInLevel error. No World Context!"));
        return;
    }

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, ActorClass, FoundActors);
    for(AActor* pActor : FoundActors)
    {
        if(URyRuntimeLevelHelpers::IsActorInLevel(pActor, level))
        {
            actorsOut.Add(pActor);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
AActor* URyRuntimeLevelHelpers::SpawnActorOfClass(UObject* WorldContextObject,
                                                  TSubclassOf<class AActor> actorClass,
                                                  const FTransform& transform,
                                                  const ESpawnActorCollisionHandlingMethod spawnHandling /*= ESpawnActorCollisionHandlingMethod::AlwaysSpawn*/,
                                                  const FName name /*= NAME_None*/,
                                                  AActor* actorTemplate /*= nullptr*/,
                                                  AActor* actorOwner /*= nullptr*/,
                                                  APawn* actorInstigator /*= nullptr*/,
                                                  ULevel* overrideLevel /*= nullptr*/)
{
    UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    if(!world)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("RyRuntimeLevelHelpers::SpawnActorOfClass error. Invalid World Context!"));
        return nullptr;
    }

    FActorSpawnParameters params;
    params.Name = name;
    params.Template = actorTemplate;
    params.Owner = actorOwner;
    params.Instigator = actorInstigator;
    params.OverrideLevel = overrideLevel;
    params.SpawnCollisionHandlingOverride = spawnHandling;
    
    return world->SpawnActor(actorClass, &transform, params);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
AActor* URyRuntimeLevelHelpers::SpawnActorOfClassDeferred(UObject* WorldContextObject,
                                                          TSubclassOf<class AActor> actorClass,
                                                          const FTransform& transform,
                                                          const ESpawnActorCollisionHandlingMethod spawnHandling /*= ESpawnActorCollisionHandlingMethod::AlwaysSpawn*/,
                                                          const FName name /*= NAME_None*/,
                                                          AActor* actorTemplate /*= nullptr*/,
                                                          AActor* actorOwner /*= nullptr*/,
                                                          APawn* actorInstigator /*= nullptr*/,
                                                          ULevel* overrideLevel /*= nullptr*/)
{
    UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    if(!world)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("RyRuntimeLevelHelpers::SpawnActorOfClassDeferred error. Invalid World Context!"));
        return nullptr;
    }

    FActorSpawnParameters params;
    params.Name = name;
    params.Template = actorTemplate;
    params.Owner = actorOwner;
    params.Instigator = actorInstigator;
    params.OverrideLevel = overrideLevel;
    params.SpawnCollisionHandlingOverride = spawnHandling;
    params.bDeferConstruction = true;
    
    return world->SpawnActor(actorClass, &transform, params);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeLevelHelpers::FinishSpawningDeferredActor(AActor* actorToFinishSpawning, const FTransform& newTransform, bool useNewTransform /*= false*/)
{
    if(!actorToFinishSpawning)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("RyRuntimeLevelHelpers::FinishSpawningDeferredActor error. actorToFinishSpawning is invalid?!"));
        return;
    }

    actorToFinishSpawning->FinishSpawning(newTransform, useNewTransform);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UActorComponent* URyRuntimeLevelHelpers::CreateComponentForActor(AActor *owner, TSubclassOf<UActorComponent> newComponentClass, 
                                                                 USceneComponent *attachComponent /*= nullptr*/, const FName newName)
{
    UActorComponent* NewInstanceComponent = NewObject<UActorComponent>(owner, newComponentClass, newName);
    if(!NewInstanceComponent)
        return nullptr;

    if(attachComponent)
    {
        USceneComponent* sceneComp = Cast<USceneComponent>(NewInstanceComponent);
        if(sceneComp)
        {
            sceneComp->AttachToComponent(attachComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
        }

        if(!owner)
        {
            owner = attachComponent->GetOwner();
        }
    }
    if(owner)
    {
        NewInstanceComponent->RegisterComponent();
    }
    return NewInstanceComponent;
}

static_assert(ERyWorldType::Inactive == (ERyWorldType)EWorldType::Inactive, "ERyWorldType is not aligned to EWorldType! Update ERyWorldType to contain all elements of EWorldType!");

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ERyWorldType URyRuntimeLevelHelpers::GetWorldType(UObject* WorldContextObject)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    FString Prefix;
    if(World)
    {
        return (ERyWorldType)World->WorldType.GetValue();
    }
    return ERyWorldType::None;
}
