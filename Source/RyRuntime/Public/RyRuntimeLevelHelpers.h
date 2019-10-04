// Copyright 2019 Ryan Sheffer. All Rights Reserved.

#pragma once

#include "RyRuntimeLevelHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to runtime levels
*/
UCLASS()
class RYRUNTIME_API URyRuntimeLevelHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    URyRuntimeLevelHelpers(const FObjectInitializer& ObjectInitializer);

    // Return the associated level this actor exists in
    UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
	static ULevel* GetActorLevel(AActor* actorIn);

    // Returns true if 'actorToCheck' is in the level 'levelToCheck'
    UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
    static bool IsActorInLevel(AActor* actorToCheck, ULevel* levelToCheck);

    // Return the name string associated with this level
    // If param 'longName' is set to true, returns the full path name of this level asset
    UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
    static FString GetLevelNameString(ULevel* levelIn, bool longName = false);

    // Return true if the level passed in is the persistent level
    UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
    static bool IsLevelPersistentLevel(ULevel* levelIn);

    // Searches the level package for an object by name
    UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
    static UObject* FindObjectInLevelByName(ULevel* levelToSearch, const FString& nameToFind);
};
