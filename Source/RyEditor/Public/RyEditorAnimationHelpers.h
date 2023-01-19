// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

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

    // Create a new montage asset containing a number of animation sequences as montage segments.
    // Also creates sections in the montage named after and aligned to the sequence lengths.
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|AnimationHelpers")
    static class UAnimMontage* CreateMontageOfSequences(const TArray<class UAnimSequence*>& AnimsIn, const FName MontageName);

    // Create montage sections from animation segments set in the montage.
    // They will be named after the animation segments sequence.
    // NOTE: This clears out all segments currently in the montage to be replaced by the sequence segment names.
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|AnimationHelpers")
    static void CreateMontageSectionsFromSegments(class UAnimMontage* MontageIn);
};
