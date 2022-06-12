// Copyright 2020-2022 Solar Storm Interactive

#include "RyEditorNotificationHelpers.h"

#include "Framework/Application/SlateApplication.h"

#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void URyEditorNotificationHelpers::CreateEditorNotification(const FText Message, const ERyNotifyCompletionState notifyType /*= ERyNotifyCompletionState::None*/,
                                                            const float fadeInDuration /*= 0.5f*/, const float expireDuration /*= 1.0f*/,
                                                            const float fadeOutDuration /*= 2.0f*/, const bool useThrobber /*= false*/,
                                                            const bool useSuccessFailIcons /*= true*/, const bool useLargeFont /*= true*/)
{
    FNotificationInfo Info(Message);
    Info.FadeInDuration = fadeInDuration;
    Info.ExpireDuration = expireDuration;
    Info.FadeOutDuration = fadeOutDuration;
    Info.bUseThrobber = useThrobber;
    Info.bUseSuccessFailIcons = useSuccessFailIcons;
    Info.bUseLargeFont = useLargeFont;
    Info.bFireAndForget = true;
    Info.bAllowThrottleWhenFrameRateIsLow = false;
    auto NotificationItem = FSlateNotificationManager::Get().AddNotification(Info);
    NotificationItem->SetCompletionState((SNotificationItem::ECompletionState)notifyType);
    NotificationItem->ExpireAndFadeout();
}
