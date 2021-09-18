// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyRuntimeObjectHelpers.generated.h"

/** Async package loading result */
UENUM(BlueprintType)
enum class ERyAsyncLoadingResult : uint8
{
	/** Package failed to load */
	Failed,
    /** Package loaded successfully */
    Succeeded,
    /** Async loading was canceled */
    Canceled
};

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to runtime objects
*/
UCLASS()
class RYRUNTIME_API URyRuntimeObjectHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
	
    /** Returns true if the Soft Object Reference is not null AND not pending kill */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers", meta = (BlueprintThreadSafe))
	static bool IsLiveSoftObjectReference(const TSoftObjectPtr<UObject>& SoftObjectReference);

    // A call to see if a package is currently loaded and load that. If not loaded, tries to load the package.
    // Package path is in this format: /Game/MyFolder/MyPackage
    // Where /Game/ is the mounting point.
    // Common mount points are:
    // /Game/* : This is your projects primary content folder
    // /Plugin/* : If a plugin has content the mounting point will be the name of the plugin
    // /Engine/* : Content found in the engine
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|ObjectHelpers")
    static UPackage* FindOrLoadPackage(const FString& PackageName);

    // Loads an object in a package
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|ObjectHelpers")
    static UObject* LoadObjectFromPackage(UPackage* package, const FString& objectName);

    // Gets the package of an object. Returns None if this object isn't part of a package
    UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers")
    static UPackage* GetPackageOfObject(UObject* object);

    // With a package, returns all objects within that package
    // NOTE: This loads every asset within the package so it can be returned.
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|ObjectHelpers")
    static void GetObjectsInPackage(UPackage* package, TArray<UObject*>& ObjectsOut);

    // A call to see if a package asset is currently loaded and load that. If not loaded, tries to load the package asset.
    // Package path is in this format: /Game/MyFolder/MyPackage.MyAsset
    // Where /Game/ is the mounting point.
    // Common mount points are:
    // /Game/* : This is your projects primary content folder
    // /Plugin/* : If a plugin has content the mounting point will be the name of the plugin
    // /Engine/* : Content found in the engine
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|ObjectHelpers")
    static UObject* LoadObject(const FString& fullObjectPath);

    DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAssetLoaded, class UObject*, Loaded);

    UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "RyRuntime|ObjectHelpers")
    static void LoadAssetPriority(UObject* WorldContextObject, TSoftObjectPtr<UObject> Asset, const int32 Priority, FOnAssetLoaded OnLoaded, FLatentActionInfo LatentInfo);

	DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnPackageLoaded, UPackage*, LoadedPackage, ERyAsyncLoadingResult, Result);

	UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "RyRuntime|ObjectHelpers")
    static void LoadPackagePriority(UObject* WorldContextObject, const FString& PackagePath, const int32 Priority, const bool BlockOnLoad, FOnPackageLoaded OnLoaded, FLatentActionInfo LatentInfo);

    // Return the parent class of a class
    UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers")
    static UClass* GetParentClass(UClass* Class);

    // Return the class hierarchy in an array ordered from children to root parent
    UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers")
    static void GetClassHierarchy(UClass* Class, TArray<UClass*>& ClassHierarchy, const bool includeSelf = true);

    // Returns the default object associated with this class.
    // WARNING: If you edit this class, it could affect created instances!
    // NOTE: This function only works if RY_INCLUDE_DANGEROUS_FUNCTIONS define is enabled.
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|ObjectHelpers")
    static UObject* GetClassDefaultObject(TSubclassOf<UObject> theClass);

    // Sets an objects property by name using reflection to a Value.
    // WARNING: You can do naughty things with this like setting native variables not exposed to blueprints. Beware!
    // NOTE: This only supports setting these types: Numeric (Int, Float, Byte, etc), Bool, LinearColor, Color, Vector, Rotator, String.
    //       With struct types like Vector, the Value string must be in a text serializable form ex (X=20, Y=30, Z=40). Use ToString on the types for examples.
    // NOTE: If the property is numeric, ensure your Value can actually work with that type. If the value can be converted but precision is lost, no warnings will occur.
    // NOTE: This function only works if RY_INCLUDE_DANGEROUS_FUNCTIONS define is enabled.
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|ObjectHelpers", meta=(AdvancedDisplay = "3"))
    static bool SetObjectPropertyValue(UObject* object, const FName PropertyName, const FString& Value, const bool PrintWarnings = true);

	/// This object is a template for another object - treat like a class default object
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers|Flags")
	static bool ObjectHasFlag_ArchetypeObject(UObject* object);

	/// Object is a CDO or Class Default Object. It exists as a template for object that will be created and can be used
	/// to get default values.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers|Flags")
	static bool ObjectHasFlag_ClassDefaultObject(UObject* object);

	/// Object has begun being destroyed
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers|Flags")
	static bool ObjectHasFlag_BeginDestroyed(UObject* object);

	/// Object has finished being destroyed
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers|Flags")
	static bool ObjectHasFlag_FinishDestroyed(UObject* object);

	/// Objects flagged with WasLoaded can be expected to have been loaded from a package and was not dynamically created.
	/// NOTE: Objects which were created for a package but not saved yet will not be set as "WasLoaded" until they are saved.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers|Flags")
	static bool ObjectHasFlag_WasLoaded(UObject* object);
};
