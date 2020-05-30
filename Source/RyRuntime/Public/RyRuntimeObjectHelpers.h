// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RyRuntimeObjectHelpers.generated.h"

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

    // Return the parent class of a class
    UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers")
    static UClass* GetParentClass(UClass* Class);

    // Return the class hierarchy in an array ordered from children to root parent
    UFUNCTION(BlueprintPure, Category = "RyRuntime|ObjectHelpers")
    static void GetClassHierarchy(UClass* Class, TArray<UClass*>& ClassHierarchy);

    UFUNCTION(BlueprintCallable, Category = "RyRuntime|ObjectHelpers")
    static UObject* GetClassDefaultObject(TSubclassOf<UObject> theClass);
};
