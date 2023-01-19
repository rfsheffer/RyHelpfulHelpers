// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyEditorNotificationHelpers.generated.h"

/**
 * Notification types
*/
UENUM(BlueprintType)
enum class ERyNotifyCompletionState : uint8
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
    UE_DEPRECATED(4.26, "CreateEditorNotification is deprecated. Please use URyRuntimeNotificationHelpers::CreateNotification instead.")
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|NotificationHelpers", meta=(DeprecatedFunction, DeprecationMessage="CreateEditorNotification is deprecated. Please use RyRuntimeNotificationHelpers::CreateNotification instead."))
    static void CreateEditorNotification(const FText Message, const ERyNotifyCompletionState notifyType = ERyNotifyCompletionState::None,
                                         const float fadeInDuration = 0.5f, const float expireDuration = 1.0f, const float fadeOutDuration = 2.0f,
                                         const bool useThrobber = false, const bool useSuccessFailIcons = true, const bool useLargeFont = true);
};
