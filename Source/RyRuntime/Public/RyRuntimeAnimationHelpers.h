// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
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
    // @param SlotOverride - The animation slot can be overridden setting this parameter. None means do nothing.
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|AnimationHelpers", meta=(AdvancedDisplay = "1"))
    static class UAnimMontage* CreateDynamicMontageFromMontage(class UAnimMontage* MontageIn, 
                                                               const FName SlotOverride = NAME_None,
                                                               const float OverrideBlendIn = -1.0f, 
                                                               const float OverrideBlendOut = -1.0f, 
                                                               const float OverrideBlendOutTriggerTime = -1.0f);

    /**
     * Create a dynamic montage containing the sequences in SequencesIn, optionally creating named sections for each sequence.
     * SequencesIn and PerSequenceSectionNames should contain the same number of elements but isn't required.
     * Duplicate section names will be ignored / skipped.
     * Sequences will only be sectioned up to length of sections array. 
     * Sequences beyond the number of section names will not be assigned a section.
     * @param SequencesIn - The sequences in order to be put in the dynamic montage
     * @param PerSequenceSectionNames - (Optional) Section names to be assigned per sequence index. This array should be aligned to SequencesIn.
     * @param LoopTimes - (Optional) Number of times to loop the sequences. This array should be aligned to SequencesIn.
     * @param AnimSlot - Specific animation slot, or none for default.
     * @param BlendIn - blend in time for this montage
     * @param BlendOut - blend out time for this montage
     * @param BlendOutTriggerTime - blend out trigger time for this montage
    */
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|AnimationHelpers", meta=(AdvancedDisplay = "1"))
    static class UAnimMontage* CreateDynamicMontageOfSequences(const TArray<class UAnimSequence*>& SequencesIn, 
                                                               const TArray<FName>& PerSequenceSectionNames,
                                                               const TArray<int32>& LoopTimes,
                                                               const FName AnimSlot = NAME_None,
                                                               const float BlendIn = 0.25f,
                                                               const float BlendOut = 0.25f,
                                                               const float BlendOutTriggerTime = -1.f,
                                                               const bool EnableAutoBlendOut = true);

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
