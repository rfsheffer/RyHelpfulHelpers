// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#pragma once

#include "RyEditorAnimationHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to animation assets
*/
UCLASS()
class RYEDITOR_API URyEditorAnimationHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    // A Helper function to create a component of a class type and attach it to the actor
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|AnimationHelpers")
    static class UAnimMontage* CreateMontageOfAnimations(const TArray<class UAnimSequence*>& AnimsIn, const FName MontageName);
};
