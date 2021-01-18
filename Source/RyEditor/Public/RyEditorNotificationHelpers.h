// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyEditorNotificationHelpers.generated.h"

/**
 * Notification types
*/
UENUM(BlueprintType)
enum ERyNotifyCompletionState
{
    None,
    Pending,
    Success,
    Fail,
};

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to editor notifications
*/
UCLASS()
class RYEDITOR_API URyEditorNotificationHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    // Create an editor notification
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|NotificationHelpers")
    static void CreateEditorNotification(const FText Message, const ERyNotifyCompletionState notifyType = ERyNotifyCompletionState::None,
                                         const float fadeInDuration = 0.5f, const float expireDuration = 1.0f, const float fadeOutDuration = 2.0f,
                                         const bool useThrobber = false, const bool useSuccessFailIcons = true, const bool useLargeFont = true);
};
