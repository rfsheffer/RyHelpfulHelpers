// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SlateCore/Public/Input/Events.h"
#include "UMG/Public/Blueprint/WidgetNavigation.h"

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
	 * Gets whether or not the cursor should always be locked to the viewport
	 */
	UFUNCTION(BlueprintPure, Category="RyRuntime|WidgetHelpers", meta=(WorldContext = "WorldContextObject", CallableWithoutWorldContext))
	static bool GetShouldAlwaysLockMouse(UObject* WorldContextObject);

	/**
	 * Gets whether or not the cursor is locked to the viewport when the viewport captures the mouse
	 */
	UFUNCTION(BlueprintPure, Category="RyRuntime|WidgetHelpers", meta=(WorldContext = "WorldContextObject", CallableWithoutWorldContext))
	static bool GetMouseLockDuringCapture(UObject* WorldContextObject);

	/**
	 * Force a on mouse leave event on a widget (can be useful for clearing the hovered state of a widget)
	 */
	UFUNCTION(BlueprintCallable, Category="RyRuntime|WidgetHelpers")
	static void ForceOnMouseLeave(class UWidget* widget, const FPointerEvent& mouseEvent);

	/**
	 * Broacast the OnClicked delegate on a button
	 * NOTE: The internal slate widget doesn't know about this event so no visual change occurs. This is only used
	 * when you want to fire the logic of the delegate and don't care about the slate internals.
	 * @param button The button
	 */
	UFUNCTION(BlueprintCallable, Category="RyRuntime|WidgetHelpers")
	static void BroadcastButtonOnClicked(class UButton* button);

	/**
	 * Broacast the OnPressed delegate on a button
	 * NOTE: The internal slate widget doesn't know about this event so no visual change occurs. This is only used
	 * when you want to fire the logic of the delegate and don't care about the slate internals.
	 * @param button The button
	 */
	UFUNCTION(BlueprintCallable, Category="RyRuntime|WidgetHelpers")
	static void BroadcastButtonOnPressed(class UButton* button);

	/**
	 * Broacast the OnReleased delegate on a button
	 * NOTE: The internal slate widget doesn't know about this event so no visual change occurs. This is only used
	 * when you want to fire the logic of the delegate and don't care about the slate internals.
	 * @param button The button
	 */
	UFUNCTION(BlueprintCallable, Category="RyRuntime|WidgetHelpers")
	static void BroadcastButtonOnReleased(class UButton* button);

	/**
	 * Broacast the OnHovered delegate on a button
	 * NOTE: The internal slate widget doesn't know about this event so no visual change occurs. This is only used
	 * when you want to fire the logic of the delegate and don't care about the slate internals.
	 * @param button The button
	 */
	UFUNCTION(BlueprintCallable, Category="RyRuntime|WidgetHelpers")
	static void BroadcastButtonOnHovered(class UButton* button);

	/**
	 * Broacast the OnUnhovered delegate on a button
	 * NOTE: The internal slate widget doesn't know about this event so no visual change occurs. This is only used
	 * when you want to fire the logic of the delegate and don't care about the slate internals.
	 * @param button The button
	 */
	UFUNCTION(BlueprintCallable, Category="RyRuntime|WidgetHelpers")
	static void BroadcastButtonOnUnhovered(class UButton* button);

	/// Get the resolved widget of a navigation data structure.
	/// NOTE: This function will also resolve custom delegates if one is assigned.
	/// NOTE: If the owning widget has not initialized yet this will not resolve anything unless it uses a custom delegate.
	/// @param navigationData The data to fetch the widget reference from
	/// @param direction The direction of this navigation data. Passed into the custom delegate if used, otherwise not important.
	UFUNCTION(BlueprintPure, Category="RyRuntime|WidgetHelpers")
	static class UWidget* GetNavigationWidget(const FWidgetNavigationData& navigationData, EUINavigation direction);

	/**
	 * Gets the next parent of a widget and if needed gets the parent widget of sub UserWidgets
	 * GetParent would usually only go up to the UserWidget root widget and no further. This routine detects the root
	 * and continues into the next owning widget if there is one.
	 * @param widget The widget to get the parent of
	 * @return The parent if there is one
	 */
	UFUNCTION(BlueprintCallable, Category="RyRuntime|WidgetHelpers")
	static class UPanelWidget* GetWidgetParent(class UWidget* widget);
};
