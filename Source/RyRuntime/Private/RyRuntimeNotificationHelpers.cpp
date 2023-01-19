// Copyright 2020-2023 Solar Storm Interactive

#include "RyRuntimeNotificationHelpers.h"

#include "Framework/Application/SlateApplication.h"

#include "Framework/Notifications/NotificationManager.h"
#include "Misc/FeedbackContext.h"
#include "Widgets/Notifications/SNotificationList.h"

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void URySlowTaskDialog::BeginDestroy()
{
	Super::BeginDestroy();
	DestroyDialog();
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
bool URySlowTaskDialog::IsValidSlowTask() const
{
	return IsInitialized && GWarn;
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
URySlowTaskDialog* URySlowTaskDialog::CreateSlowTaskDialog(FText DefaultMessage, FText FrameMessage,
	float TotalAmountOfWork, float CompletedWork, float CurrentFrameScope, bool MakeDialogImmediately)
{
	if(!GWarn)
	{
		return nullptr;
	}
	
	URySlowTaskDialog* taskDialog = NewObject<URySlowTaskDialog>();
	if(!taskDialog)
	{
		return nullptr;
	}
	
	taskDialog->SlowTask.Initialize();
	taskDialog->IsInitialized = true;
	taskDialog->SlowTask.CompletedWork = CompletedWork;
	taskDialog->SlowTask.TotalAmountOfWork = TotalAmountOfWork;
	taskDialog->SlowTask.CurrentFrameScope = CurrentFrameScope;
	taskDialog->SlowTask.DefaultMessage = DefaultMessage;
	taskDialog->SlowTask.FrameMessage = FrameMessage;

	if(MakeDialogImmediately)
	{
		taskDialog->DialogUp = true;
		taskDialog->SlowTask.MakeDialog();
	}

	return taskDialog;
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void URySlowTaskDialog::MakeDialog()
{
	if(IsValidSlowTask() && !DialogUp)
	{
		DialogUp = true;
		SlowTask.MakeDialog();
	}
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void URySlowTaskDialog::EnterProgressFrame(const float ExpectedWorkThisFrame, const FText Text)
{
	if(IsValidSlowTask())
	{
		SlowTask.EnterProgressFrame(ExpectedWorkThisFrame, Text);
	}
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void URySlowTaskDialog::SetTotalAmountOfWork(float TotalAmountOfWork)
{
	SlowTask.TotalAmountOfWork = TotalAmountOfWork;
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void URySlowTaskDialog::SetCompletedWork(float CompletedWork)
{
	SlowTask.CompletedWork = CompletedWork;
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void URySlowTaskDialog::SetCurrentFrameScope(float CurrentFrameScope)
{
	SlowTask.CurrentFrameScope = CurrentFrameScope;
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void URySlowTaskDialog::DestroyDialog()
{
	if(IsValidSlowTask() && GWarn && GWarn->GetScopeStack().Num() > 0)
	{
		IsInitialized = false;
		SlowTask.Destroy();
	}
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeNotificationHelpers::CreateNotification(const FText Message, const ERyRunNotifyCompletionState notifyType /*= ERyRunNotifyCompletionState::None*/,
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

//--------------------------------------------------------------------------------------------------------------------
/**
*/
URySlowTaskDialog* URyRuntimeNotificationHelpers::CreateSlowTaskDialog(FText DefaultMessage, FText FrameMessage,
	float TotalAmountOfWork, float CompletedWork, float CurrentFrameScope, bool MakeDialogImmediately)
{
	return URySlowTaskDialog::CreateSlowTaskDialog(DefaultMessage, FrameMessage, TotalAmountOfWork, CompletedWork, CurrentFrameScope, MakeDialogImmediately);
}
