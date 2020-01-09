// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
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

    // Return the associated level this actor exists in
    UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
	static ULevel* GetActorLevel(const AActor* actorIn);

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
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers")
    static UObject* FindObjectInLevelByName(ULevel* levelToSearch, const FString& nameToFind);

    // Return the associated level this actor exists in
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers")
	static void GetActorsOfTypeInLevel(ULevel* level, TSubclassOf<AActor> ActorClass, TArray<AActor*>& actorsOut);

    // A Helper function to create a component of a class type and attach it to the actor at runtime
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers")
    static class UActorComponent* CreateComponentForActor(AActor *owner, TSubclassOf<class UActorComponent> newComponentClass,
                                                          class USceneComponent *attachComponent = nullptr);
};
