// Copyright 2020-2023 Solar Storm Interactive

#include "RyRuntimeWidgetHelpers.h"
#include "RyRuntimeModule.h"
#include "Components/Button.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "UMG/Public/Components/Widget.h"

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

#undef LOCTEXT_NAMESPACE
