// Copyright 2020-2022 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyRuntimeNotificationHelpers.generated.h"

/**
 * Notification types
*/
UENUM(BlueprintType)
enum class ERyRunNotifyCompletionState : uint8
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
class RYRUNTIME_API URyRuntimeNotificationHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/// Create an editor notification
	/// NOTE: The platforms this function works will vary. More than likely only desktop.
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|NotificationHelpers")
	static void CreateNotification(const FText Message, const ERyRunNotifyCompletionState notifyType = ERyRunNotifyCompletionState::None,
										 const float fadeInDuration = 0.5f, const float expireDuration = 1.0f, const float fadeOutDuration = 2.0f,
										 const bool useThrobber = false, const bool useSuccessFailIcons = true, const bool useLargeFont = true);
};
