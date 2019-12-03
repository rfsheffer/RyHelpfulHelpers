// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#pragma once

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

    // A Helper function to create a component of a class type and attach it to the actor
    UFUNCTION(BlueprintCallable, Category = "RyEditor|LevelHelpers")
    static UActorComponent* CreateComponentForActor(AActor *owner, UClass* newComponentClass);
};
