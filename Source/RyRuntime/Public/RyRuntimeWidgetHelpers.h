// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyRuntimeWidgetHelpers.generated.h"

/**
 * 
 */
UCLASS()
class RYRUNTIME_API URyRuntimeWidgetHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * Setup an input mode that allows only player input / player controller to respond to user input.
	 * This version does not consume the mouse down event, and instead it is passed to player input processing
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="Input")
    static void SetInputMode_GameOnly_NoMouseDownConsume(APlayerController* PlayerController);
};
