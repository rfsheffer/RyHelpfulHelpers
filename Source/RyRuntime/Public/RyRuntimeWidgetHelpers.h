// Copyright 2020-2022 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SlateCore/Public/Input/Events.h"

#include "RyRuntimeWidgetHelpers.generated.h"

/**
 * 
 */
UCLASS()
class RYRUNTIME_API URyRuntimeWidgetHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Setup an input mode that allows only player input / player controller to respond to user input.
	 * This version does not consume the mouse down event, and instead it is passed to player input processing
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="RyRuntime|WidgetHelpers")
    static void SetInputMode_GameOnly_NoMouseDownConsume(APlayerController* PlayerController);

	/**
	 * Force a on mouse leave event on a widget (can be useful for clearing the hovered state of a widget)
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="RyRuntime|WidgetHelpers")
	static void ForceOnMouseLeave(class UWidget* widget, const FPointerEvent& mouseEvent);
};
