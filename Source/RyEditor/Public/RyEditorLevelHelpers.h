// Copyright 2020-2022 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyEditorLevelHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to editor levels
*/
UCLASS()
class RYEDITOR_API URyEditorLevelHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    URyEditorLevelHelpers(const FObjectInitializer& ObjectInitializer);

    /// A Helper function to create a component of a class type and attach it to the actor
	/// This version is for creating a component to attach to an actor in an editor context. It will so the usual
	/// work to setup the component to be attached to your actor and saved with your level.
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|LevelHelpers")
    static UActorComponent* CreateComponentForEditorActor(AActor *owner, TSubclassOf<UActorComponent> newComponentClass, USceneComponent *attachComponent = nullptr);
};
