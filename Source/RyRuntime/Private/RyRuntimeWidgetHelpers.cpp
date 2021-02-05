// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeWidgetHelpers.h"
#include "RyRuntimeModule.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

#define LOCTEXT_NAMESPACE "UMG"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeWidgetHelpers::SetInputMode_GameOnly_NoMouseDownConsume(APlayerController* PlayerController)
{
	if (PlayerController != nullptr)
	{
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(false);
		PlayerController->SetInputMode(InputMode);
	}
#if WITH_EDITOR 
	else
	{
		FMessageLog("PIE").Error(LOCTEXT("UMG WidgetBlueprint Library: SetInputMode_GameOnly_NoConsumeMouse", "SetInputMode_GameOnly_NoConsumeMouse expects a valid player controller as 'PlayerController' target"));
	}
#endif // WITH_EDITOR
}

#undef LOCTEXT_NAMESPACE
