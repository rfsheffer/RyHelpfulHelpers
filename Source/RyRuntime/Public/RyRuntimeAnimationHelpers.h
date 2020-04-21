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
};
