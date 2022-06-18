// Copyright 2020-2022 Solar Storm Interactive

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/LatentActionManager.h"

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

	/** Returns string representation of reference, in form /package/path.assetname */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers", meta = (BlueprintThreadSafe))
	static FString SoftObjectToString(const TSoftObjectPtr<UObject>& SoftObjectReference);

	/**  
	* Test if this does not point to a live UObject, but may in the future
	*
	* @return true if this does not point to a real object, but could possibly
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers", meta = (BlueprintThreadSafe))
	static bool IsSoftObjectPending(const TSoftObjectPtr<UObject>& SoftObjectReference);

	/**  
	* Test if this can never point to a live UObject
	*
	* @return true if this is explicitly pointing to no object
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers", meta = (BlueprintThreadSafe))
	static bool IsSoftObjectNull(const TSoftObjectPtr<UObject>& SoftObjectReference);

	/**  
	* Test if this points to a live UObject
	*
	* @return true if Get() would return a valid non-null pointer
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers", meta = (BlueprintThreadSafe))
	static bool IsSoftObjectValid(const TSoftObjectPtr<UObject>& SoftObjectReference);

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

	/** 
	 * Tries to convert the supplied relative or absolute filename to a long package name/path starting with a root like /game
	 * This works on both package names and directories, and it does not validate that it actually exists on disk.
	 * 
	 * @param InFilename Filename to convert.
	 * @param OutPackageName The resulting long package name if the conversion was successful.
	 * @param OutFailureReason Description of an error if the conversion failed.
	 * @return Returns true if the supplied filename properly maps to one of the long package roots.
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ObjectHelpers")
	static bool TryConvertFilenameToLongPackageName(const FString& InFilename, FString& OutPackageName, FString& OutFailureReason);

    /// A call to see if a package asset is currently loaded and load that. If not loaded, tries to load the package asset.
    /// Package path is in this format: /Game/MyFolder/MyPackage.MyAsset
    /// Where /Game/ is the mounting point.
    /// Common mount points are:
    /// /Game/* : This is your projects primary content folder
    /// /Plugin/* : If a plugin has content the mounting point will be the name of the plugin
    /// /Engine/* : Content found in the engine
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|ObjectHelpers")
    static UObject* LoadObject(const FString& fullObjectPath);

    DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAssetLoaded, class UObject*, Loaded);

    UFUNCTION(BlueprintCallable, meta = (Latent = "", LatentInfo = "LatentInfo", WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "RyRuntime|ObjectHelpers")
    static void LoadAssetPriority(UObject* WorldContextObject, const TSoftObjectPtr<UObject> Asset, const int32 Priority, FOnAssetLoaded OnLoaded, FLatentActionInfo LatentInfo);

	DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnPackageLoaded, UPackage*, LoadedPackage, ERyAsyncLoadingResult, Result);

	UFUNCTION(BlueprintCallable, meta = (Latent = "", LatentInfo = "LatentInfo", WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "RyRuntime|ObjectHelpers")
    static void LoadPackagePriority(UObject* WorldContextObject, const FString& PackagePath, const int32 Priority, const bool BlockOnLoad, FOnPackageLoaded OnLoaded, FLatentActionInfo LatentInfo);

	/**
	 * This will insert a mount point at the head of the search chain (so it can overlap an existing mount point and win).
	 * This function can be used to mount a path to a simple mounting point name. ex:
	 * ../../../MyProject/Plugins/MyDLC/Content to /MyDLC/
	 *
	 * @param RootPath Logical Root Path. ex: /MyDLC/
	 * @param ContentPath Content Path on disk. ex: ../../../MyProject/Plugins/MyDLC/Content
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ObjectHelpers")
	static void RegisterMountPoint(const FString& RootPath, const FString& ContentPath);

	/**
	 * This will remove a previously inserted mount point.
	 *
	 * @param RootPath Logical Root Path.
	 * @param ContentPath Content Path on disk.
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ObjectHelpers")
	static void UnRegisterMountPoint(const FString& RootPath, const FString& ContentPath);

	/**
	 * Returns whether the specific logical root path is a valid mount point.
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers")
	static bool MountPointExists(const FString& RootPath);

	/**
	 * Get the mount point for a given package path
	 * 
	 * @param InPackagePath The package path to get the mount point for
	 * @param InWithoutSlashes Optional parameters that keeps the slashes around the mount point if false
	 * @return FName corresponding to the mount point, or Empty if invalid
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers")
	static FName GetPackageMountPoint(const FString& InPackagePath, bool InWithoutSlashes = true);
	
    // Return the parent class of a class
    UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers")
    static UClass* GetParentClass(UClass* Class);

    // Return the class hierarchy in an array ordered from children to root parent
    UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers")
    static void GetClassHierarchy(UClass* Class, TArray<UClass*>& ClassHierarchy, const bool includeSelf = true);

    // Returns the default object associated with this class.
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|ObjectHelpers")
    static UObject* GetClassDefaultObject(TSubclassOf<UObject> theClass);

    // Sets an objects property by name using reflection to a Value.
    // WARNING: You can do naughty things with this like setting native variables not exposed to blueprints. Beware!
    // NOTE: This only supports setting these types: Numeric (Int, Float, Byte, etc), Bool, LinearColor, Color, Vector, Rotator, String.
    //       With struct types like Vector, the Value string must be in a text serializable form ex (X=20, Y=30, Z=40). Use ToString on the types for examples.
    // NOTE: If the property is numeric, ensure your Value can actually work with that type. If the value can be converted but precision is lost, no warnings will occur.
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
