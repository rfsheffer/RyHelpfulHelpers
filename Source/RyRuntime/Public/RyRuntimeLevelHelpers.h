// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"

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

UENUM(BlueprintType)
enum class ERyComponentCreationMethod : uint8
{
	/** A component that is part of a native class. */
	Native,
    /** A component that is created from a template defined in the Components section of the Blueprint. */
    SimpleConstructionScript,
    /**A dynamically created component, either from the UserConstructionScript or from a Add Component node in a Blueprint event graph. */
    UserConstructionScript,
    /** A component added to a single Actor instance via the Component section of the Actor's details panel. */
    Instance,
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
	// Strips any UEDPIE_N_" prefix or _LevelInstance_ postfix
	UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
	static TSoftObjectPtr<UWorld> GetWorldSoftReferenceFromPath(const FString& PathToWorld);

	// Returns the world object of an actor
	UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
	static UWorld* GetWorldOfActor(const AActor* actorIn);

	// Creates a soft reference for a world object
	UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
    static TSoftObjectPtr<UWorld> GetWorldSoftReference(UWorld* worldIn);

	/// Get the world soft reference path
	/// Strips any UEDPIE_N_" prefix or _LevelInstance_ postfix
	UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers", meta=(AutoCreateRefTerm="worldRef"))
	static FString GetWorldSoftReferencePath(const TSoftObjectPtr<UWorld>& worldRef);

	/// Get the world soft reference
	/// Strips any UEDPIE_N_" prefix or _LevelInstance_ postfix
	UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers", meta=(AutoCreateRefTerm="worldRef"))
	static TSoftObjectPtr<UWorld> GetCleanWorldSoftReference(const TSoftObjectPtr<UWorld>& worldRef);

    // Return the associated level this actor exists in
    UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
	static ULevel* GetActorLevel(const AActor* actorIn);

	/** Get the name string of the world this actor is in. This is not the sub level name, use GetActorLevelName for that. */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
	static FString GetActorWorldNameString(const AActor* actorIn);

    // Returns true if 'actorToCheck' is in the level 'levelToCheck'
    UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
    static bool IsActorInLevel(const AActor* actorToCheck, const ULevel* levelToCheck);

	// Removes extra name decorations to paths, ex : "/Path/MyLevels/UEDPIE_N_MyLevel_LevelInstance_N" becomes "/Path/MyLevels/MyLevel"
	// Removes "UEDPIE_N_" and "_LevelInstance_N", whichever is found or both.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|LevelHelpers")
	static FString RemoveLevelPathDecorators(const FString& levelNamePath);

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
	// This function is designed to open up the advanced control usually only available in C++. Lets you specify a level,
	// template, deferred construction, name, etc.
	// @param actorClass - The class of the actor to spawn
	// @param transform - The world transform applied to the actor being spawned
	// @param spawnHandling - How spawning should be handled regarding world geometry
	// @param name - (Optional) name to assign. If None, will get an auto generated named
	// @param actorTemplate - (Optional) An Actor to use as a template when spawning the new Actor. The spawned Actor will be initialized using the property values of the template Actor. If left NULL the class default object (CDO) will be used to initialize the spawned Actor
	// @param actorOwner - (Optional) The Actor that spawned this Actor
	// @param actorInstigator - (Optional) The APawn that is responsible for damage done by the spawned Actor
	// @param overrideLevel - (Optional) The ULevel to spawn the Actor in, i.e. the Outer of the Actor. If left as NULL the Outer of the Owner is used. If the Owner is NULL the persistent level is used
	// @param deferConstruction - If True, WILL NOT run Construction Script of Blueprints to give caller an opportunity to set parameters beforehand.
	//							  Caller is responsible for invoking construction manually by calling FinishSpawningActor.
	// @param allowDuringConstructionScript - If true you can spawn actors during construction script. This is can be very useful for spawning helper actors but make sure you are cleaning up after yourself or bad things can happen!
	// @param absoluteTransform - Use absolute transform (override root component transform) and SpawnParameters.
	// @param useDefaultScale - When spawning should the actors original scale of the root transform be used instead of the transform passed in?
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers", meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "3", CallableWithoutWorldContext, DeterminesOutputType = "actorClass"))
	static class AActor* SpawnActorAdvanced(UObject* WorldContextObject,
										   TSubclassOf<class AActor> actorClass,
										   const FTransform& transform,
										   const ESpawnActorCollisionHandlingMethod spawnHandling = ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
										   const FName name = NAME_None,
										   AActor* actorTemplate = nullptr,
										   AActor* actorOwner = nullptr,
										   APawn* actorInstigator = nullptr,
										   ULevel* overrideLevel = nullptr,
										   bool deferConstruction = false,
										   bool allowDuringConstructionScript = false,
                                           bool absoluteTransform = true,
                                           bool useDefaultScale = false);

	// Finish spawning an actor that was created via SpawnActorAdvanced with deferConstruction set to true.
	// @param actorToFinishSpawning - The actor to finish spawning
	// @param newTransform - (Optional) A new transform to apply to this actor
	// @param useNewTransform - (Optional) If true, newTransform will be applied to this actor, else, the transform used in SpawnActorOfClassDeferred will be used.
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers", meta = (AdvancedDisplay = "1"))
	static void FinishSpawnActorAdvanced(AActor* actorToFinishSpawning, const FTransform& newTransform, bool useNewTransform = false);

    /**
     * A Helper function to create a component of a class type and attach it to the actor at runtime. This does not support presenting exposed variables.
     * @param owner - The actor to assign this component to
     * @param newComponentClass - The class of compoent to spawn
     * @param attachComponent - The component to attach this component to (if this component is a scene component)
     * @param newName - (Optional) new name
     * @param creationMethod - Creation method. See enum values for details.
     * @param allowAnyoneToDestroy - By default components can only be destroyed by thier owners. If this is true, this component can be destroyed by anything.
     */
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers", meta = (AdvancedDisplay = "3", DeterminesOutputType = "newComponentClass"))
    static class UActorComponent* CreateComponentForActor(AActor *owner, TSubclassOf<class UActorComponent> newComponentClass,
                                                          class USceneComponent *attachComponent = nullptr, const FName newName = NAME_None,
                                                          const ERyComponentCreationMethod creationMethod = ERyComponentCreationMethod::UserConstructionScript,
                                                          const bool allowAnyoneToDestroy = false);

	/// With an un-registered component, register it with an actor
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers")
	static void RegisterComponentForActor(class UActorComponent* componentToRegister, AActor* actor, class USceneComponent *attachComponent = nullptr);

	// Allows you to change the creation method of a component. WARNING: This can be dangerous and should only be used to fixup errors.
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers")
	static void SetComponentCreationMethod(class UActorComponent* componentToSet, const ERyComponentCreationMethod creationMethod);

	/// Set a component to be destroyable by anyone. This goes against the nature of components in Unreal, but sometimes you just want momentary components...
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers")
	static void SetAllowAnyoneToDestroyMe(class UActorComponent* componentToSet, const bool can);
	
    /// Get the type of world the context object is in (editor, preview, game, PlayInEditor[PIE], etc)
    UFUNCTION(BlueprintPure, Category = "RyRuntime|WorldHelpers", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
	static ERyWorldType GetWorldType(UObject* WorldContextObject);

	/**
	* Full implementation of the Actor TeleportTo call for Blueprints
	* Used for adding actors to levels or teleporting them to a new location.
	* The result of this function is independent of the actor's current location and rotation.
	* If the actor doesn't fit exactly at the location specified, tries to slightly move it out of walls and such if bNoCheck is false.
	*
	* @param ActorToTeleport The actor to teleport
	* @param DestLocation The target destination point
	* @param DestRotation The target rotation at the destination
	* @param IsATest is true if this is a test movement, which shouldn't cause any notifications (used by AI pathfinding, for example)
	* @param NoCheck is true if we should skip checking for encroachment in the world or other actors
	* @return true if the actor has been successfully moved, or false if it couldn't fit.
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|LevelHelpers")
    static bool TeleportToAdvanced(AActor *ActorToTeleport, const FVector& DestLocation, const FRotator& DestRotation, bool IsATest=false, bool NoCheck=false);

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

	/** Tries to find an event named "EventName" on all level scripts, and calls it */
	UFUNCTION(BlueprintCallable, Category="RyRuntime|LevelScriptActor", meta=(WorldContext="WorldContextObject"))
    static bool FireLevelScriptRemoteEvent(UObject* WorldContextObject, FName EventName);

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
