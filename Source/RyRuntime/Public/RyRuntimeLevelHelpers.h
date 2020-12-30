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

UENUM(BlueprintType)
enum class ERyCurrentLevelStreamingState : uint8
{
	Removed,
    Unloaded,
    FailedToLoad,
    Loading,
    LoadedNotVisible,
    MakingVisible,
    LoadedVisible,
    MakingInvisible
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

	// Given a full path to your world ex: "/Game/MyLevels/MyLevel" return a constructed soft object ptr.
	// This is used in cases such as the LevelStreamingDynamics need for a soft object pointer to a World
	// which cannot be constructed dynamically.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
	static TSoftObjectPtr<UWorld> GetWorldSoftObjectPtr(const FString& PathToWorld);

    // Return the associated level this actor exists in
    UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
	static ULevel* GetActorLevel(const AActor* actorIn);

    // Returns true if 'actorToCheck' is in the level 'levelToCheck'
    UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
    static bool IsActorInLevel(const AActor* actorToCheck, const ULevel* levelToCheck);

	// Return the level package string associated with this actor
	// @param longName - If set to true, returns the full path name of the level asset (In PIE sessions, this will be the duplicate level name)
	UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers", meta = (AdvancedDisplay = "1"))
    static FString GetActorLevelPackageString(const AActor* actorIn, bool longName = true);

	// Return the level package string associated with this level
	// @param longName - If set to true, returns the full path name of the level asset (In PIE sessions, this will be the duplicate level name)
    UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers", meta = (AdvancedDisplay = "1"))
    static FString GetLevelPackageString(const ULevel* levelIn, bool longName = true);

	// Returns the name of the level (Always the correct name, no PIE name change)
	UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
	static FString GetLevelNameString(const ULevel* levelIn);

	// Returns the name of the level associated with this actor (Always the correct name, no PIE name change)
	UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
	static FString GetActorLevelNameString(const AActor* actorIn);

    // Return true if the level passed in is the persistent level
    UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
    static bool IsLevelPersistentLevel(const ULevel* levelIn);

    // Searches the level package for an object by name
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers")
    static UObject* FindObjectInLevelByName(ULevel* levelToSearch, const FString& nameToFind);

    // Gets all actors of ActorClass in a specific level
    // This is a slow operation, use with caution e.g. do not use every frame.
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers")
	static void GetActorsOfTypeInLevel(const ULevel* level, TSubclassOf<AActor> ActorClass, TArray<AActor*>& actorsOut);
	
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
	// @param allowDuringConstructionScript - If true you can spawn actors during construction script. This is can be very useful for spawning helper actors but make sure you are cleaning up after yourself or bad things can happen!
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers", meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "3", CallableWithoutWorldContext))
	static class AActor* SpawnActorOfClass(UObject* WorldContextObject,
										   TSubclassOf<class AActor> actorClass,
										   const FTransform& transform,
										   const ESpawnActorCollisionHandlingMethod spawnHandling = ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
										   const FName name = NAME_None,
										   AActor* actorTemplate = nullptr,
										   AActor* actorOwner = nullptr,
										   APawn* actorInstigator = nullptr,
										   ULevel* overrideLevel = nullptr,
										   bool allowDuringConstructionScript = false);

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
	// @param allowDuringConstructionScript - If true you can spawn actors during construction script. This is can be very useful for spawning helper actors but make sure you are cleaning up after yourself or bad things can happen!
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers", meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "3", CallableWithoutWorldContext))
    static class AActor* SpawnActorOfClassDeferred(UObject* WorldContextObject,
    											   TSubclassOf<class AActor> actorClass,
										           const FTransform& transform,
										           const ESpawnActorCollisionHandlingMethod spawnHandling = ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
										           const FName name = NAME_None,
										           AActor* actorTemplate = nullptr,
										           AActor* actorOwner = nullptr,
										           APawn* actorInstigator = nullptr,
										           ULevel* overrideLevel = nullptr,
                                                   bool allowDuringConstructionScript = false);

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

	/**  
	* Stream in a level with a specific location and rotation.
	* This is an advanced implementation with extended control of how the level is loaded.
	*
	* The level to be loaded does not have to be in the persistent map's Levels list, however to ensure that the .umap does get
	* packaged, please be sure to include the .umap in your Packaging Settings:
	*
	*   Project Settings -> Packaging -> List of Maps to Include in a Packaged Build (you may have to show advanced or type in filter)
	*
	* @param WorldContextObject - The world context, to get a pointer to the underlying world to stream this level into
	* @param LevelName - Level package name, ex: /Game/Maps/MyMapName, specifying short name like MyMapName will force very slow search on disk
	* @param Location - World space location where the level should be spawned
	* @param Rotation - World space rotation for rotating the entire level
	* @param OutSuccess - Whether operation was successful (map was found and added to the sub-levels list)
	* @param LevelPrefix - Prefix for the dynamic level instance (If an empty string, will be changed to "_LevelInstance_")
	* @param ShouldBeLoaded - Should the level be loaded?
	* @param ShouldBeVisible - Should the level be visible once loaded?
	* @param BlockOnLoad - Should the level load asynchronously or block and load synchronously?
	* @param Priority - Sets the relative priority of considering the streaming level. Changing the priority will not interrupt the currently considered level, but will affect the next time a level is being selected for evaluation.
	* @return Streaming level object for a level instance
	*/ 
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelStreaming", meta=(DisplayName = "Load Level Instance Advanced (by Name)", WorldContext="WorldContextObject", AdvancedDisplay = "5"))
    static class ULevelStreamingDynamic* LoadLevelInstanceAdvanced(UObject* WorldContextObject,
    															   FString LevelName,
    															   FVector Location,
    															   FRotator Rotation,
    															   bool& OutSuccess,
    															   const FString& LevelPrefix = TEXT("_LevelInstance_"),
    															   const bool ShouldBeLoaded = true,
    															   const bool ShouldBeVisible = true,
    															   const bool BlockOnLoad = false,
    															   const int32 Priority = 0);

	/**  
	* Stream in a level with a specific location and rotation.
	* This is an advanced implementation with extended control of how the level is loaded.
	*
	* The level to be loaded does not have to be in the persistent map's Levels list, however to ensure that the .umap does get
	* packaged, please be sure to include the .umap in your Packaging Settings:
	*
	*   Project Settings -> Packaging -> List of Maps to Include in a Packaged Build (you may have to show advanced or type in filter)
	*
	* @param WorldContextObject - The world context, to get a pointer to the underlying world to stream this level into
	* @param Level - The soft object point to the level to load.
	* @param Location - World space location where the level should be spawned
	* @param Rotation - World space rotation for rotating the entire level
	* @param OutSuccess - Whether operation was successful (map was found and added to the sub-levels list)
	* @param LevelPrefix - Prefix for the dynamic level instance (If an empty string, will be changed to "_LevelInstance_")
	* @param ShouldBeLoaded - Should the level be loaded?
	* @param ShouldBeVisible - Should the level be visible once loaded?
	* @param BlockOnLoad - Should the level load asynchronously or block and load synchronously?
	* @param Priority - Sets the relative priority of considering the streaming level. Changing the priority will not interrupt the currently considered level, but will affect the next time a level is being selected for evaluation.
	* @return Streaming level object for a level instance
	*/ 
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelStreaming", meta=(DisplayName = "Load Level Instance Advanced (by Object Reference)", WorldContext="WorldContextObject", AdvancedDisplay = "5"))
    static class ULevelStreamingDynamic* LoadLevelInstanceBySoftObjectPtrAdvanced(UObject* WorldContextObject,
    																			  TSoftObjectPtr<UWorld> Level,
    																			  FVector Location,
    																			  FRotator Rotation,
    																			  bool& OutSuccess,
    																			  const FString& LevelPrefix = TEXT("_LevelInstance_"),
                                                                                  const bool ShouldBeLoaded = true,
                                                                                  const bool ShouldBeVisible = true,
                                                                                  const bool BlockOnLoad = false,
                                                                                  const int32 Priority = 0);

	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelStreaming")
	static ERyCurrentLevelStreamingState GetCurrentLevelStreamingState(class ULevelStreaming* StreamingLevel);

	/** Returns the Level Script Actor of the level if the level is loaded and valid */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelStreaming")
    static class ALevelScriptActor* GetStreamingLevelScriptActor(class ULevelStreaming* StreamingLevel);

private:
	// Counter used by LoadLevelInstance to create unique level names
	static int32 UniqueLevelInstanceId;

	static ULevelStreamingDynamic* LoadLevelInstance_Internal(UWorld* World,
															  const FString& LongPackageName,
															  FVector Location,
															  FRotator Rotation,
															  bool& OutSuccess,
															  const FString& LevelPrefix,
                                                              const bool ShouldBeLoaded,
                                                              const bool ShouldBeVisible,
                                                              const bool BlockOnLoad,
                                                              const int32 Priority);
};
