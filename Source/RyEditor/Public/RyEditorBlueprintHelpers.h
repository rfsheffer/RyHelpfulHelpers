// Copyright 2020-2022 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyEditorBlueprintHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to blueprints
*/
UCLASS()
class RYEDITOR_API URyEditorBlueprintHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    /**
     * Creates a blueprint or returns the blueprint at the packagePath location.
     * @param packagePath: The package path where the blueprint should be created. ex: /Game/Blueprints/BP_MyBlueprint
     * @param parentClass: The parent class of this blueprint.
     * @param getExistingBlueprint: Will return the blueprint at the package path if one already exists. Will warn about mismatch class.
     * @return The Created or blueprint at the location if getExistingBlueprint is true
    */
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|BlueprintHelpers")
    static class UBlueprint* CreateBlueprint(const FString& packagePath, TSubclassOf<class UObject> parentClass, bool getExistingBlueprint);

    /**
     * Get a blueprint asset at a package path.
    */
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|BlueprintHelpers")
    static class UBlueprint* GetBlueprint(const FString& packagePath);

    /**
     * Get a blueprints generated class
    */
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|BlueprintHelpers")
    static class UBlueprintGeneratedClass* GetBlueprintGeneratedClass(class UBlueprint* blueprint);

    /**
     * Get a blueprints generated class at a package path
    */
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|BlueprintHelpers")
    static class UBlueprintGeneratedClass* GetBlueprintGeneratedClassAtPath(const FString& packagePath);

    /**
     * Compile a blueprint
    */
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|BlueprintHelpers")
    static bool CompileBlueprint(class UBlueprint* blueprint, TArray<FString>& compilerMessagesOut);

    /**
     * Add components (Created using CreateComponentForActor) to a blueprint.
    */
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|BlueprintHelpers")
    static void AddComponentsToBlueprint(UBlueprint* blueprint, TArray<UActorComponent*> Components, bool bHarvesting = false, bool bKeepMobility = false);
};
