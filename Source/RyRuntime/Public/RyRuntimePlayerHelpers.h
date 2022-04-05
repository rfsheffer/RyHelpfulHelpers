// Copyright 2020-2022 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"

#include "RyRuntimePlayerHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
* Static Helper functions for the player.
*/
UCLASS()
class RYRUNTIME_API URyRuntimePlayerHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	// Has camera fading occured before?
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlayerHelpers")
	static bool IsCameraFadingEnabled(APlayerCameraManager* cameraManager);
	
	// Return true if the camera manager is currently fading the active camera
	// Value is only valid if a fade had occured. Check IsCameraFadingEnabled.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlayerHelpers")
    static bool IsCameraFading(APlayerCameraManager* cameraManager);

	// Return true if the camera is currently faded by some amount.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlayerHelpers")
    static bool IsCameraFaded(APlayerCameraManager* cameraManager);

	// Get the current fade alpha of the camera
	// Value is only valid if a fade had occured. Check IsCameraFadingEnabled.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlayerHelpers")
	static float GetCameraFadeAmount(APlayerCameraManager* cameraManager);

	// Get the current fade color (if fading, returns the lerped color value, else returns the full fade color)
	// Value is only valid if a fade had occured. Check IsCameraFadingEnabled.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlayerHelpers")
	static FLinearColor GetCameraFadeColor(APlayerCameraManager* cameraManager);

	// Returns the camera manager associated with a pawn if it has one.
	// This only returns a non-None value if the pawn is controller by a player controller and the player controller has a camera manager.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlayerHelpers")
	static APlayerCameraManager* GetPawnCameraManager(APawn* pawn);
};

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimePlayerHelpers::IsCameraFadingEnabled(APlayerCameraManager* cameraManager)
{
	if(!cameraManager)
	{
		return false;
	}

	return cameraManager->bEnableFading;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimePlayerHelpers::IsCameraFading(APlayerCameraManager* cameraManager)
{
	if(!cameraManager || !cameraManager->bEnableFading)
	{
		return false;
	}

	return cameraManager->FadeTimeRemaining > 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimePlayerHelpers::IsCameraFaded(APlayerCameraManager* cameraManager)
{
	if(!cameraManager || !cameraManager->bEnableFading)
	{
		return false;
	}
	
	if(cameraManager->FadeAmount == 0.0f)
	{
		return false;
	}
	
	return cameraManager->FadeAlpha.X > 0.0f || cameraManager->FadeAlpha.Y > 0.0f;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE float URyRuntimePlayerHelpers::GetCameraFadeAmount(APlayerCameraManager* cameraManager)
{
	if(!cameraManager || !cameraManager->bEnableFading)
	{
		return 0.0f;
	}

	return cameraManager->FadeAmount;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE FLinearColor URyRuntimePlayerHelpers::GetCameraFadeColor(APlayerCameraManager* cameraManager)
{
	if(!cameraManager || !cameraManager->bEnableFading)
	{
		return FLinearColor::Black;
	}

	if(cameraManager->FadeTimeRemaining > 0)
	{
		return cameraManager->FadeColor * cameraManager->FadeAmount;
	}

	return cameraManager->FadeColor;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE APlayerCameraManager* URyRuntimePlayerHelpers::GetPawnCameraManager(APawn* pawn)
{
	if(!pawn)
	{
		return nullptr;
	}

	APlayerController* playerController = Cast<APlayerController>(pawn->GetController());
	if(!playerController)
	{
		return nullptr;
	}

	return playerController->PlayerCameraManager;
}
