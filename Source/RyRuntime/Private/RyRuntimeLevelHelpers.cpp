// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeLevelHelpers.h"
#include "RyRuntimeModule.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/PackageName.h"
#include "Engine/Level.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/LevelStreaming.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/LevelScriptActor.h"

// Ensure our blueprintable version of the enum is aligned with epics
static_assert(ERyComponentCreationMethod::Native == static_cast<ERyComponentCreationMethod>(EComponentCreationMethod::Native) &&
              ERyComponentCreationMethod::Instance == static_cast<ERyComponentCreationMethod>(EComponentCreationMethod::Instance),
              "ERyComponentCreationMethod is not aligned to EComponentCreationMethod! Update ERyComponentCreationMethod to contain all elements of EComponentCreationMethod!");

//---------------------------------------------------------------------------------------------------------------------
/**
*/
TSoftObjectPtr<UWorld> URyRuntimeLevelHelpers::GetWorldSoftReferenceFromPath(const FString& PathToWorld)
{
    return TSoftObjectPtr<UWorld>(PathToWorld);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UWorld* URyRuntimeLevelHelpers::GetWorldOfActor(const AActor* actorIn)
{
    if(actorIn)
    {
        return actorIn->GetWorld();
    }

    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
TSoftObjectPtr<UWorld> URyRuntimeLevelHelpers::GetWorldSoftReference(UWorld* worldIn)
{
    return TSoftObjectPtr<UWorld>(worldIn);
}

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
FString URyRuntimeLevelHelpers::GetActorWorldNameString(const AActor* actorIn)
{
    if(!actorIn || !actorIn->GetWorld())
    {
        return TEXT("");
    }

    return actorIn->GetWorld()->GetName();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeLevelHelpers::IsActorInLevel(const AActor* actorToCheck, const ULevel* levelToCheck)
{
    if(actorToCheck == nullptr || levelToCheck == nullptr)
        return false;

    return GetActorLevel(actorToCheck) == levelToCheck;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeLevelHelpers::GetActorLevelPackageString(const AActor* actorIn, bool longName)
{
    ULevel* level = GetActorLevel(actorIn);
    if(level)
    {
        return GetLevelPackageString(level, longName);
    }
    return TEXT("");
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeLevelHelpers::GetLevelPackageString(const ULevel* levelIn, bool longName)
{
    if(levelIn == nullptr || levelIn->GetOutermost() == nullptr)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("URyRuntimeLevelHelpers::Get*PackageString called with invalid levelIn or actor!"));
        return TEXT("");
    }

    return longName ? levelIn->GetOutermost()->GetName() : FPackageName::GetShortName(levelIn->GetOutermost()->GetFName());
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeLevelHelpers::GetLevelNameString(const ULevel* levelIn)
{
    if(levelIn == nullptr || levelIn->GetOutermost() == nullptr)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("URyRuntimeLevelHelpers::Get*LevelNameString called with invalid levelIn or actor!"));
        return TEXT("");
    }
    return levelIn->GetOuter()->GetName();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeLevelHelpers::GetActorLevelNameString(const AActor* actorIn)
{
    ULevel* level = GetActorLevel(actorIn);
    if(level)
    {
        return GetLevelNameString(level);
    }
    
    return TEXT("");
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeLevelHelpers::IsLevelPersistentLevel(const ULevel* levelIn)
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
    return StaticFindObject(/*Class=*/ nullptr, levelToSearch, *nameToFind, true);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeLevelHelpers::GetActorsOfTypeInLevel(const ULevel* level, TSubclassOf<AActor> ActorClass, TArray<AActor*>& actorsOut)
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
AActor* URyRuntimeLevelHelpers::SpawnActorAdvanced(UObject* WorldContextObject,
                                                  TSubclassOf<class AActor> actorClass,
                                                  const FTransform& transform,
                                                  const ESpawnActorCollisionHandlingMethod spawnHandling /*= ESpawnActorCollisionHandlingMethod::AlwaysSpawn*/,
                                                  const FName name /*= NAME_None*/,
                                                  AActor* actorTemplate /*= nullptr*/,
                                                  AActor* actorOwner /*= nullptr*/,
                                                  APawn* actorInstigator /*= nullptr*/,
                                                  ULevel* overrideLevel /*= nullptr*/,
                                                  bool deferConstruction,
                                                  bool allowDuringConstructionScript,
                                                  bool absoluteTransform,
                                                  bool useDefaultScale)
{
    UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    if(!world)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("RyRuntimeLevelHelpers::SpawnActorAdvanced error. Invalid World Context!"));
        return nullptr;
    }

    FActorSpawnParameters params;
    params.Name = name;
    params.Template = actorTemplate;
    params.Owner = actorOwner;
    params.Instigator = actorInstigator;
    params.OverrideLevel = overrideLevel;
    params.SpawnCollisionHandlingOverride = spawnHandling;
    params.bAllowDuringConstructionScript = allowDuringConstructionScript;
    params.bDeferConstruction = deferConstruction;
    params.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;

    // For a bit of extra security, ensure the template class overrides whatever the user puts in.
    // This allows for the class field to be empty.
    if(actorTemplate)
    {
        actorClass = actorTemplate->GetClass();
    }

    if(!actorClass)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("RyRuntimeLevelHelpers::SpawnActorAdvanced error. Invalid actor class!"));
        return nullptr;
    }

    AActor* spawnedActor;
    if(absoluteTransform)
    {
        spawnedActor = world->SpawnActorAbsolute(actorClass, transform, params);
    }
    else
    {
        spawnedActor = world->SpawnActor(actorClass, &transform, params);
    }

#if WITH_EDITOR
    if(spawnedActor)
    {
        spawnedActor->SetActorLabel(spawnedActor->GetName());
    }
#endif

    if(spawnedActor && useDefaultScale)
    {
        AActor* cdoActor = spawnedActor->GetClass()->GetDefaultObject<AActor>();
        if(cdoActor)
        {
            spawnedActor->SetActorScale3D(cdoActor->GetActorScale3D());
        }
    }
    
    return spawnedActor;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeLevelHelpers::FinishSpawnActorAdvanced(AActor* actorToFinishSpawning, const FTransform& newTransform, bool useNewTransform /*= false*/)
{
    if(!actorToFinishSpawning)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("RyRuntimeLevelHelpers::FinishSpawnActorAdvanced error. actorToFinishSpawning is invalid?!"));
        return;
    }

    actorToFinishSpawning->FinishSpawning(newTransform, useNewTransform);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UActorComponent* URyRuntimeLevelHelpers::CreateComponentForActor(AActor *owner, TSubclassOf<UActorComponent> newComponentClass, 
                                                                 USceneComponent *attachComponent /*= nullptr*/, const FName newName,
                                                                 const ERyComponentCreationMethod creationMethod,
                                                                 const bool allowAnyoneToDestroy)
{
    UActorComponent* NewInstanceComponent = NewObject<UActorComponent>(owner, newComponentClass, newName);
    if(!NewInstanceComponent)
    {
        return nullptr;
    }

    NewInstanceComponent->bAllowAnyoneToDestroyMe = allowAnyoneToDestroy;
    NewInstanceComponent->CreationMethod = static_cast<EComponentCreationMethod>(creationMethod);

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

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeLevelHelpers::RegisterComponentForActor(UActorComponent* componentToRegister, AActor* actor, USceneComponent* attachComponent)
{
    if(!actor || !componentToRegister)
    {
        return;
    }

    if(componentToRegister->IsRegistered())
    {
        return;
    }

    if(attachComponent)
    {
        USceneComponent* sceneComp = Cast<USceneComponent>(componentToRegister);
        if(sceneComp)
        {
            sceneComp->AttachToComponent(attachComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
        }

        if(!actor)
        {
            actor = attachComponent->GetOwner();
        }
    }
    if(actor)
    {
        componentToRegister->RegisterComponent();
    }
}

static_assert(ERyWorldType::None == static_cast<ERyWorldType>(EWorldType::None) &&
              ERyWorldType::Inactive == static_cast<ERyWorldType>(EWorldType::Inactive), "ERyWorldType is not aligned to EWorldType! Update ERyWorldType to contain all elements of EWorldType!");

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ERyWorldType URyRuntimeLevelHelpers::GetWorldType(UObject* WorldContextObject)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    FString Prefix;
    if(World)
    {
        return static_cast<ERyWorldType>(World->WorldType.GetValue());
    }
    return ERyWorldType::None;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeLevelHelpers::TeleportToAdvanced(AActor* ActorToTeleport, const FVector& DestLocation,
    const FRotator& DestRotation, bool IsATest, bool NoCheck)
{
    if(!ActorToTeleport)
    {
        return false;
    }

    return ActorToTeleport->TeleportTo(DestLocation, DestRotation, IsATest, NoCheck);
}

int32 URyRuntimeLevelHelpers::UniqueLevelInstanceId = 0;

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ULevelStreamingDynamic* URyRuntimeLevelHelpers::LoadLevelInstanceAdvanced(UObject* WorldContextObject,
                                                                          FString LevelName,
                                                                          FVector Location,
                                                                          FRotator Rotation,
                                                                          bool& OutSuccess,
                                                                          const FString& LevelPrefix,
                                                                          const bool ShouldBeLoaded,
                                                                          const bool ShouldBeVisible,
                                                                          const bool BlockOnLoad,
                                                                          const int32 Priority)
{
    FString prefixIn = LevelPrefix;
    if(prefixIn.IsEmpty())
    {
        prefixIn = TEXT("_LevelInstance_");
    }

    OutSuccess = false;
    UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        return nullptr;
    }

    // Check whether requested map exists, this could be very slow if LevelName is a short package name
    FString LongPackageName;
    OutSuccess = FPackageName::SearchForPackageOnDisk(LevelName, &LongPackageName);
    if (!OutSuccess)
    {
        return nullptr;
    }

    return LoadLevelInstance_Internal(World, LongPackageName, Location, Rotation, OutSuccess, LevelPrefix, ShouldBeLoaded, ShouldBeVisible, BlockOnLoad, Priority);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ULevelStreamingDynamic* URyRuntimeLevelHelpers::LoadLevelInstanceBySoftObjectPtrAdvanced(UObject* WorldContextObject,
                                                                                         TSoftObjectPtr<UWorld> Level,
                                                                                         FVector Location,
                                                                                         FRotator Rotation,
                                                                                         bool& OutSuccess,
                                                                                         const FString& LevelPrefix,
                                                                                         const bool ShouldBeLoaded,
                                                                                         const bool ShouldBeVisible,
                                                                                         const bool BlockOnLoad,
                                                                                         const int32 Priority)
{
    FString prefixIn = LevelPrefix;
    if(prefixIn.IsEmpty())
    {
        prefixIn = TEXT("_LevelInstance_");
    }

    OutSuccess = false;
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        return nullptr;
    }

    // Check whether requested map exists, this could be very slow if LevelName is a short package name
    if (Level.IsNull())
    {
        return nullptr;
    }

    return LoadLevelInstance_Internal(World, Level.GetLongPackageName(), Location, Rotation, OutSuccess, LevelPrefix, ShouldBeLoaded, ShouldBeVisible, BlockOnLoad, Priority);
}

static_assert(ERyCurrentLevelStreamingState::MakingInvisible ==
    static_cast<ERyCurrentLevelStreamingState>(ULevelStreaming::ECurrentState::MakingInvisible), "ERyCurrentLevelStreamingState is not aligned to ECurrentState! Update ERyCurrentLevelStreamingState to contain all elements of ECurrentState!");


//---------------------------------------------------------------------------------------------------------------------
/**
*/
ERyCurrentLevelStreamingState URyRuntimeLevelHelpers::GetCurrentLevelStreamingState(ULevelStreaming* StreamingLevel)
{
    if(!StreamingLevel)
    {
        return ERyCurrentLevelStreamingState::Removed;
    }

    return static_cast<ERyCurrentLevelStreamingState>(StreamingLevel->GetCurrentState());
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ALevelScriptActor* URyRuntimeLevelHelpers::GetStreamingLevelScriptActor(ULevelStreaming* StreamingLevel)
{
    if(!StreamingLevel)
    {
        return nullptr;
    }

    return StreamingLevel->GetLevelScriptActor();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeLevelHelpers::FireLevelScriptRemoteEvent(UObject* WorldContextObject, FName EventName)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        return false;
    }
    
    bool bFoundEvent = false;

    // Iterate over all levels, and try to find a matching function on the level's script actor
    for( TArray<ULevel*>::TConstIterator it = World->GetLevels().CreateConstIterator(); it; ++it )
    {
        ULevel* CurLevel = *it;
        if( CurLevel && CurLevel->bIsVisible )
        {
            ALevelScriptActor* LSA = CurLevel->GetLevelScriptActor();
            if( LSA )
            {
                // Find an event with no parameters
                UFunction* EventTarget = LSA->FindFunction(EventName);
                if( EventTarget && EventTarget->NumParms == 0)
                {
                    LSA->ProcessEvent(EventTarget, nullptr);
                    bFoundEvent = true;
                }
            }
        }
    }

    return bFoundEvent;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ULevelStreamingDynamic* URyRuntimeLevelHelpers::LoadLevelInstance_Internal(UWorld* World,
                                                                           const FString& LongPackageName,
                                                                           FVector Location,
                                                                           FRotator Rotation,
                                                                           bool& OutSuccess,
                                                                           const FString& LevelPrefix,
                                                                           const bool ShouldBeLoaded,
                                                                           const bool ShouldBeVisible,
                                                                           const bool BlockOnLoad,
                                                                           const int32 Priority)
{
    const FString PackagePath = FPackageName::GetLongPackagePath(LongPackageName);
    FString ShortPackageName = FPackageName::GetShortName(LongPackageName);

    if (ShortPackageName.StartsWith(World->StreamingLevelsPrefix))
    {
        ShortPackageName.RightChopInline(World->StreamingLevelsPrefix.Len(), false);
    }

    // Remove PIE prefix if it's there before we actually load the level
    const FString OnDiskPackageName = FString::Printf(TEXT("%s/%s"), *PackagePath, *ShortPackageName);

    // Create Unique Name for sub-level package
    const FString UniqueLevelPackageName = FString::Printf(TEXT("%s/%s%s%s%d"),
                                                           *PackagePath,
                                                           *World->StreamingLevelsPrefix,
                                                           *ShortPackageName,
                                                           *LevelPrefix,
                                                           ++UniqueLevelInstanceId);
    
    // Setup streaming level object that will load specified map
    ULevelStreamingDynamic* StreamingLevel = NewObject<ULevelStreamingDynamic>(World, ULevelStreamingDynamic::StaticClass(), NAME_None, RF_Transient, nullptr);
    StreamingLevel->SetWorldAssetByPackageName(FName(*UniqueLevelPackageName));
    StreamingLevel->LevelColor = FColor::MakeRandomColor();
    StreamingLevel->SetShouldBeLoaded(ShouldBeLoaded);
    StreamingLevel->SetShouldBeVisible(ShouldBeVisible);
    StreamingLevel->SetPriority(Priority);
    StreamingLevel->bShouldBlockOnLoad = BlockOnLoad;
    StreamingLevel->bInitiallyLoaded = ShouldBeLoaded;
    StreamingLevel->bInitiallyVisible = ShouldBeVisible;
    // Transform
    StreamingLevel->LevelTransform = FTransform(Rotation, Location);
    // Map to Load
    StreamingLevel->PackageNameToLoad = FName(*OnDiskPackageName);
          
    // Add the new level to world.
    World->AddStreamingLevel(StreamingLevel);
      
    OutSuccess = true;
    return StreamingLevel;
}
