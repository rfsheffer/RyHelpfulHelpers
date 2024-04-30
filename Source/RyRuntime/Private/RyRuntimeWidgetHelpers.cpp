// Copyright 2020-2023 Solar Storm Interactive

#include "RyRuntimeWidgetHelpers.h"
#include "RyRuntimeModule.h"
#include "Components/Button.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Components/Widget.h"
#include "Runtime/Engine/Classes/Engine/GameViewportClient.h"
#include "GenericPlatform/GenericApplication.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "WidgetHelpers"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeWidgetHelpers::SetInputMode_GameOnly_NoMouseDownConsume(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		UE_LOG(LogRyRuntime, Error, TEXT("Calling SetInputMode_GameOnly_NoMouseDownConsume with no valid player controller!"))
		return;
	}

	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	PlayerController->SetInputMode(InputMode);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeWidgetHelpers::GetShouldAlwaysLockMouse(UObject* WorldContextObject)
{
	if(!WorldContextObject)
	{
		return false;
	}

	const UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World)
	{
		return false;
	}
	
	UGameViewportClient* GameViewportClient = World->GetGameViewport();
	if(!GameViewportClient)
	{
		return false;
	}

	return GameViewportClient->ShouldAlwaysLockMouse();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeWidgetHelpers::GetMouseLockDuringCapture(UObject* WorldContextObject)
{
	if(!WorldContextObject)
	{
		return false;
	}

	const UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World)
	{
		return false;
	}
	
	UGameViewportClient* GameViewportClient = World->GetGameViewport();
	if(!GameViewportClient)
	{
		return false;
	}

	return GameViewportClient->LockDuringCapture();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeWidgetHelpers::ForceOnMouseLeave(UWidget* widget, const FPointerEvent& mouseEvent)
{
	if(!widget)
	{
		return;
	}

	TSharedPtr<SWidget> slateWidget = widget->GetCachedWidget();
	if(slateWidget)
	{
		slateWidget->OnMouseLeave(mouseEvent);
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeWidgetHelpers::BroadcastButtonOnClicked(UButton* button)
{
	if(button)
	{
		button->OnClicked.Broadcast();
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeWidgetHelpers::BroadcastButtonOnPressed(UButton* button)
{
	if(button)
	{
		button->OnPressed.Broadcast();
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeWidgetHelpers::BroadcastButtonOnReleased(UButton* button)
{
	if(button)
	{
		button->OnReleased.Broadcast();
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeWidgetHelpers::BroadcastButtonOnHovered(UButton* button)
{
	if(button)
	{
		button->OnHovered.Broadcast();
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeWidgetHelpers::BroadcastButtonOnUnhovered(UButton* button)
{
	if(button)
	{
		button->OnUnhovered.Broadcast();
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UWidget* URyRuntimeWidgetHelpers::GetNavigationWidget(const FWidgetNavigationData& navigationData, EUINavigation direction)
{
	if(navigationData.CustomDelegate.IsBound())
	{
		return navigationData.CustomDelegate.Execute(direction);
	}
	return navigationData.Widget.Get(false);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UPanelWidget* URyRuntimeWidgetHelpers::GetWidgetParent(UWidget* widget)
{
	if(!widget)
	{
		return nullptr;
	}

	if(widget->GetParent())
	{
		return widget->GetParent();
	}

	if(!widget->GetOuter())
	{
		return nullptr;
	}

	const UUserWidget* userWidget = Cast<UUserWidget>(widget->GetOuter()->GetOuter());
	if(!userWidget)
	{
		return nullptr;
	}

	return userWidget->GetParent();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeWidgetHelpers::GetUserWidgetSlotNames(UUserWidget* userWidget, TArray<FName>& SlotNames)
{
	if(userWidget)
	{
		userWidget->GetSlotNames(SlotNames);
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UWidget* URyRuntimeWidgetHelpers::GetUserWidgetContentForSlot(UUserWidget* userWidget, const FName slotName)
{
	if(userWidget)
	{
		return userWidget->GetContentForSlot(slotName);
	}
	return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UWidget* URyRuntimeWidgetHelpers::GetUserWidgetRootWidget(UUserWidget* userWidget)
{
	if(userWidget)
	{
		return userWidget->GetRootWidget();
	}
	return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeWidgetHelpers::IsCapsLock()
{
	return FSlateApplication::Get().GetModifierKeys().AreCapsLocked();
}

#undef LOCTEXT_NAMESPACE
