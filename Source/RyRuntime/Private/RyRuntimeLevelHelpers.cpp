// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#include "RyRuntimePrivatePCH.h"
#include "RyRuntimeLevelHelpers.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
URyRuntimeLevelHelpers::URyRuntimeLevelHelpers(const FObjectInitializer& ObjectInitializer)
                            : Super(ObjectInitializer)
{

}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ULevel* URyRuntimeLevelHelpers::GetActorLevel(const AActor* actorIn)
{
    if(!actorIn || !actorIn->IsValidLowLevelFast(true) || actorIn->IsPendingKill())
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("URyRuntimeLevelHelpers::GetActorLevel called with invalid actorIn!"));
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
        UE_LOG(LogRyRuntime, Error, TEXT("RyRuntimeLevelHelpers error. level is NULL!"));
        return;
    }

    UWorld* const World = level->GetWorld();
    if(!World)
    {
        UE_LOG(LogRyRuntime, Error, TEXT("RyRuntimeLevelHelpers error. No World Context!"));
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
UActorComponent* URyRuntimeLevelHelpers::CreateComponentForActor(AActor *owner, TSubclassOf<UActorComponent> newComponentClass, 
                                                                 USceneComponent *attachComponent /*= nullptr*/)
{
    UActorComponent* NewInstanceComponent = NewObject<UActorComponent>(owner, newComponentClass);
    if(!NewInstanceComponent)
        return nullptr;

    if(attachComponent)
    {
        USceneComponent* sceneComp = Cast<USceneComponent>(NewInstanceComponent);
        if(sceneComp)
        {
            sceneComp->AttachToComponent(attachComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
        }
    }
    NewInstanceComponent->RegisterComponent();
    return NewInstanceComponent;
}
