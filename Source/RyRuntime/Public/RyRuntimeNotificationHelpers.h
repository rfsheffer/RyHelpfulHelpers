// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Core/Public/Misc/SlowTask.h"

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

/**
 * Slow Task dialog object
 */
UCLASS(NotBlueprintable, BlueprintType)
class URySlowTaskDialog : public UObject
{
	GENERATED_BODY()
public:

	URySlowTaskDialog() :
	    IsInitialized(false)
	  , DialogUp(false)
	  , SlowTask(0)
	{
	}

	URySlowTaskDialog(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	  , IsInitialized(false)
	  , DialogUp(false)
	  , SlowTask(0)
	{
	}
    
	/** DO NOT USE. This constructor is for internal usage only for statically-created objects. */
	URySlowTaskDialog(EStaticConstructor InE, EObjectFlags InFlags) : Super(InE, InFlags)
	  , IsInitialized(false)
	  , DialogUp(false)
	  , SlowTask(0)
	{
	}
    
    /** DO NOT USE. This constructor is for internal usage only for hot-reload purposes. */
	URySlowTaskDialog(FVTableHelper& Helper) : Super(Helper)
	  , IsInitialized(false)
	  , DialogUp(false)
	  , SlowTask(0)
	{
	}

	virtual void BeginDestroy() override;

	bool IsValidSlowTask() const;

	static URySlowTaskDialog* CreateSlowTaskDialog(FText DefaultMessage, FText FrameMessage, float TotalAmountOfWork = 0,
		float CompletedWork = 0, float CurrentFrameScope = 0, bool MakeDialogImmediately = true);

	UFUNCTION(BlueprintCallable, Category="RySlowTaskDialog")
	void MakeDialog();

	UFUNCTION(BlueprintCallable, Category="RySlowTaskDialog")
	void EnterProgressFrame(const float ExpectedWorkThisFrame = 1.f, const FText Text = FText());

	UFUNCTION(BlueprintCallable, Category="RySlowTaskDialog")
	void SetTotalAmountOfWork(float TotalAmountOfWork = 0);

	UFUNCTION(BlueprintPure, Category="RySlowTaskDialog")
	float GetTotalAmountOfWork() const
	{
		return SlowTask.TotalAmountOfWork;
	}

	UFUNCTION(BlueprintCallable, Category="RySlowTaskDialog")
	void SetCompletedWork(float CompletedWork = 0);

	UFUNCTION(BlueprintPure, Category="RySlowTaskDialog")
	float GetCompletedWork() const
	{
		return SlowTask.CompletedWork;
	}

	UFUNCTION(BlueprintCallable, Category="RySlowTaskDialog")
	void SetCurrentFrameScope(float CurrentFrameScope = 0);

	UFUNCTION(BlueprintPure, Category="RySlowTaskDialog")
	float GetCurrentFrameScope() const
	{
		return SlowTask.CurrentFrameScope;
	}

	UFUNCTION(BlueprintCallable, Category="RySlowTaskDialog")
	void DestroyDialog();

private:

	bool IsInitialized;
	bool DialogUp;
	FSlowTask SlowTask;
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

	/**
	 * Create a 'SlowTask' Dialog. This is a dialog like any loading dialog. You define the amount of work that will be done and over time report work being completed.
	 * A dialog is created with information about the work. This looks like any Unreal Engine loading dialog.
	 * @param DefaultMessage - Default message to display to the user when not overridden by a frame
	 * @param FrameMessage - Message pertaining to the current frame's work
	 * @param TotalAmountOfWork - The amount of work to do in this scope
	 * @param CompletedWork - The amount of work we have already completed in this scope
	 * @param CurrentFrameScope - The amount of work the current frame is responsible for
	 * @param MakeDialogImmediately - If true will Make the dialog immediately, else you need to call MakeDialog on the URySlowTaskDialog yourself.
	 * @return The slow task dialog object which when destroyed will kill the dialog.
	*/
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|NotificationHelpers")
	static URySlowTaskDialog* CreateSlowTaskDialog(FText DefaultMessage, FText FrameMessage, float TotalAmountOfWork = 0,
		float CompletedWork = 0, float CurrentFrameScope = 0, bool MakeDialogImmediately = true);
};
