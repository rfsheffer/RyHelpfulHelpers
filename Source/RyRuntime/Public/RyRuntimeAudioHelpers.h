// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyRuntimeAudioHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
* Static Helper functions related to audio
*/
UCLASS()
class RYRUNTIME_API URyRuntimeAudioHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	// Gets a list of available audio devices
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|AudioHelpers")
	static void GetAudioDeviceList(TArray<FString>& OutAudioDeviceNames);

	// Gets a list of available audio capture devices
	// NOTE: Only Supports Windows at the moment
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|AudioHelpers")
    static void GetAudioCaptureDeviceList(TArray<FString>& OutAudioCaptureDeviceNames);

	TArray<FString> CachedCaptureDeviceList;
};
