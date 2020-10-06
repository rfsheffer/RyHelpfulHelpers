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
	
	// A Helper function to create a actor of a class type. This does not support presenting exposed variables.
	// Use SpawnActorOfClassDeferred to modify settings pre BeginPlay.
	// @param actorClass - The class of the actor to spawn
	// @param transform - The world transform applied to the actor being spawned
	// @param spawnHandling - How spawning should be handled regarding world geometry
	// @param name - (Optional) name to assign. If None, will get an auto generated named
	// @param actorTemplate - (Optional) An Actor to use as a template when spawning the new Actor. The spawned Actor will be initialized using the property values of the template Actor. If left NULL the class default object (CDO) will be used to initialize the spawned Actor
	// @param actorOwner - (Optional) The Actor that spawned this Actor
	// @param actorInstigator - (Optional) The APawn that is responsible for damage done by the spawned Actor
	// @param overrideLevel - (Optional) The ULevel to spawn the Actor in, i.e. the Outer of the Actor. If left as NULL the Outer of the Owner is used. If the Owner is NULL the persistent level is used
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers", meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "3", CallableWithoutWorldContext))
	static class AActor* SpawnActorOfClass(UObject* WorldContextObject,
										   TSubclassOf<class AActor> actorClass,
										   const FTransform& transform,
										   const ESpawnActorCollisionHandlingMethod spawnHandling = ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
										   const FName name = NAME_None,
										   AActor* actorTemplate = nullptr,
										   AActor* actorOwner = nullptr,
										   APawn* actorInstigator = nullptr,
										   ULevel* overrideLevel = nullptr);

	// A Helper function to create a actor of a class type. This does not support presenting exposed variables.
	// Make sure to call FinishSpawningDeferredActor on the returned actor.
	// This function is useful for creating an actor, making modification, then finishing the spawning process via FinishSpawningDeferredActor.
	// @param actorClass - The class of the actor to spawn
	// @param transform - The world transform applied to the actor being spawned
	// @param spawnHandling - How spawning should be handled regarding world geometry
	// @param name - (Optional) name to assign. If None, will get an auto generated named
	// @param actorTemplate - (Optional) An Actor to use as a template when spawning the new Actor. The spawned Actor will be initialized using the property values of the template Actor. If left NULL the class default object (CDO) will be used to initialize the spawned Actor
	// @param actorOwner - (Optional) The Actor that spawned this Actor
	// @param actorInstigator - (Optional) The APawn that is responsible for damage done by the spawned Actor
	// @param overrideLevel - (Optional) The ULevel to spawn the Actor in, i.e. the Outer of the Actor. If left as NULL the Outer of the Owner is used. If the Owner is NULL the persistent level is used
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers", meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "3", CallableWithoutWorldContext))
    static class AActor* SpawnActorOfClassDeferred(UObject* WorldContextObject,
    											   TSubclassOf<class AActor> actorClass,
										           const FTransform& transform,
										           const ESpawnActorCollisionHandlingMethod spawnHandling = ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
										           const FName name = NAME_None,
										           AActor* actorTemplate = nullptr,
										           AActor* actorOwner = nullptr,
										           APawn* actorInstigator = nullptr,
										           ULevel* overrideLevel = nullptr);

	// Finish spawning an actor that was created via SpawnActorOfClassDeferred.
	// @param actorToFinishSpawning - The actor to finish spawning
	// @param newTransform - (Optional) A new transform to apply to this actor
	// @param useNewTransform - (Optional) If true, newTransform will be applied to this actor, else, the transform used in SpawnActorOfClassDeferred will be used.
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers", meta = (AdvancedDisplay = "1"))
	static void FinishSpawningDeferredActor(AActor* actorToFinishSpawning, const FTransform& newTransform, bool useNewTransform = false);

    // A Helper function to create a component of a class type and attach it to the actor at runtime. This does not support presenting exposed variables.
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers")
    static class UActorComponent* CreateComponentForActor(AActor *owner, TSubclassOf<class UActorComponent> newComponentClass,
                                                          class USceneComponent *attachComponent = nullptr, const FName newName = NAME_None);

    // Get the type of world the context object is in (editor, preview, game, PlayInEditor[PIE], etc)
    UFUNCTION(BlueprintPure, Category = "RyRuntime|WorldHelpers", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
	static ERyWorldType GetWorldType(UObject* WorldContextObject);
};
