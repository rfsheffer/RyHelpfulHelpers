// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyRuntimeLevelHelpers.generated.h"

// An blueprintable enum type which corresponds with the EWorldType
UENUM(BlueprintType)
enum class ERyWorldType : uint8
{
    /** An untyped world, in most cases this will be the vestigial worlds of streamed in sub-levels */
    None,

    /** The game world */
    Game,

    /** A world being edited in the editor */
    Editor,

    /** A Play In Editor world */
    PIE,

    /** A preview world for an editor tool */
    EditorPreview,

    /** A preview world for a game */
    GamePreview,

    /** A minimal RPC world for a game */
    GameRPC,

    /** An editor world that was loaded but not currently being edited in the level editor */
    Inactive
};

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

    // Gets all actors of ActorClass in a specific level
    // This is a slow operation, use with caution e.g. do not use every frame.
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers")
	static void GetActorsOfTypeInLevel(ULevel* level, TSubclassOf<AActor> ActorClass, TArray<AActor*>& actorsOut);

    // A Helper function to create a component of a class type and attach it to the actor at runtime
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers")
    static class UActorComponent* CreateComponentForActor(AActor *owner, TSubclassOf<class UActorComponent> newComponentClass,
                                                          class USceneComponent *attachComponent = nullptr, const FName newName = NAME_None);

    // Get the type of world the context object is in (editor, preview, game, PlayInEditor[PIE], etc)
    UFUNCTION(BlueprintPure, Category = "RyRuntime|WorldHelpers", meta = (WorldContext = "WorldContextObject"))
	static ERyWorldType GetWorldType(UObject* WorldContextObject);
};
