// Copyright 2020-2022 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeWidgetHelpers.h"
#include "RyRuntimeModule.h"
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

#undef LOCTEXT_NAMESPACE
