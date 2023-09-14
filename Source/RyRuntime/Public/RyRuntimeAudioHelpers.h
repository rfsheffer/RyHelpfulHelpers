﻿// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "AudioCaptureDeviceInterface.h"

#include "RyRuntimeAudioHelpers.generated.h"

/// A structure inline with Audio::FCaptureDeviceInfo from AudioCaptureCore
USTRUCT(BlueprintType)
struct FRyCaptureDeviceInfo
{
	GENERATED_BODY()
	
	FRyCaptureDeviceInfo() = default;

	FRyCaptureDeviceInfo(const FString& inDeviceName, const FString& inDeviceId, int32 inInputChannels, int32 inPreferredSampleRate, bool inSupportsHardwareAEC)
		: DeviceName(inDeviceName)
		, DeviceId(inDeviceId)
		, InputChannels(inInputChannels)
		, PreferredSampleRate(inPreferredSampleRate)
		, SupportsHardwareAEC(inSupportsHardwareAEC)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CaptureDeviceInfo")
	FString DeviceName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CaptureDeviceInfo")
	FString DeviceId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CaptureDeviceInfo")
	int32 InputChannels = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CaptureDeviceInfo")
	int32 PreferredSampleRate = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CaptureDeviceInfo")
	bool SupportsHardwareAEC = false;
};

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
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|AudioHelpers")
    static void GetAudioCaptureDeviceList(TArray<FRyCaptureDeviceInfo>& OutAudioCaptureDevices);

private:
	TUniquePtr<Audio::IAudioCaptureStream> AudioCaptureStream;
};
