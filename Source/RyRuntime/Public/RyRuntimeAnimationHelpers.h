// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#pragma once

#include "RyRuntimeAnimationHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to animation assets
*/
UCLASS()
class RYRUNTIME_API URyRuntimeAnimationHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    // Creates a duplicate montage from another montage. This is useful for cases where you want to modify a montage asset at runtime.
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|AnimationHelpers")
    static class UAnimMontage* CreateDynamicMontageFromMontage(class UAnimMontage* MontageIn);

    // Get the names of sections in this montage
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|AnimationHelpers")
    static void GetMontageSectionNames(class UAnimMontage* MontageIn, TArray<FName>& NamesOut);

    // Return true if a montage has a named section
    UFUNCTION(BlueprintPure, Category = "RyRuntime|AnimationHelpers")
    static bool MontageHasSection(class UAnimMontage* MontageIn, const FName SectionName);

    // Return true if a montage has a named section
    UFUNCTION(BlueprintPure, Category = "RyRuntime|AnimationHelpers")
    static FName GetMontageSectionNameFromPosition(class UAnimMontage* MontageIn, const float Position);

    // Return true if a montage has a named section
    UFUNCTION(BlueprintPure, Category = "RyRuntime|AnimationHelpers")
    static void GetMontageSectionStartAndEndTime(class UAnimMontage* MontageIn, const FName SectionName, float& OutStartTime, float& OutEndTime);

    UFUNCTION(BlueprintPure, Category = "RyRuntime|AnimationHelpers")
    static float GetMontageSectionTimeLeftFromPos(class UAnimMontage* MontageIn, const FName SectionName, const float Position);
};
