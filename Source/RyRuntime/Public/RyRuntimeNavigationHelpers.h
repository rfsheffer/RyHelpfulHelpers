// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyRuntimeNavigationHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to runtime levels
*/
UCLASS()
class RYRUNTIME_API URyRuntimeNavigationHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    /** box extent used ONLY when owning actor doesn't have collision component */
    UFUNCTION(BlueprintPure, Category = "RyRuntime|NavigationHelpers")
	static FVector GetFailsafeExtent(class UNavModifierComponent* NavModifierComponent);
};
