// Copyright 2020-2023 Solar Storm Interactive


#include "RyRuntimeAudioHelpers.h"
#include "RyRuntimeModule.h"
#include "Runtime/Engine/Public/AudioDevice.h"
#include "AudioCaptureCore.h"

#if PLATFORM_WINDOWS
#include "Runtime/Online/Voice/Private/VoicePrivate.h"
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeAudioHelpers::GetAudioDeviceList(TArray<FString>& OutAudioDeviceNames)
{
	FAudioDeviceHandle audioDevice = FAudioDevice::GetMainAudioDevice();
	if(audioDevice.IsValid())
	{
		audioDevice->GetAudioDeviceList(OutAudioDeviceNames);
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeAudioHelpers::GetAudioCaptureDeviceList(TArray<FRyCaptureDeviceInfo>& OutAudioCaptureDevices)
{
	OutAudioCaptureDevices.Reset();
	URyRuntimeAudioHelpers* audioHelpersCOD = StaticClass()->GetDefaultObject<URyRuntimeAudioHelpers>();
	if(audioHelpersCOD)
	{
		if(!audioHelpersCOD->AudioCaptureStream)
		{
			TArray<Audio::IAudioCaptureFactory*> AudioCaptureStreamFactories =
				IModularFeatures::Get().GetModularFeatureImplementations<Audio::IAudioCaptureFactory>(Audio::IAudioCaptureFactory::GetModularFeatureName());

			// For now, just return the first audio capture stream implemented. We can make this configurable at a later point.
			if (AudioCaptureStreamFactories.Num() > 0 && AudioCaptureStreamFactories[0] != nullptr)
			{
				audioHelpersCOD->AudioCaptureStream = AudioCaptureStreamFactories[0]->CreateNewAudioCaptureStream();
			}
		}

		if(audioHelpersCOD->AudioCaptureStream)
		{
			TArray<Audio::FCaptureDeviceInfo> OutDevices;
			audioHelpersCOD->AudioCaptureStream->GetInputDevicesAvailable(OutDevices);
			for(const Audio::FCaptureDeviceInfo& deviceInfo : OutDevices)
			{
				OutAudioCaptureDevices.Emplace(deviceInfo.DeviceName, deviceInfo.DeviceId, deviceInfo.InputChannels, deviceInfo.PreferredSampleRate, deviceInfo.bSupportsHardwareAEC);
			}
		}
		else
		{
			UE_LOG(LogRyRuntime, Display, TEXT("GetAudioCaptureDeviceList: No Audio Capture implementations found for this platform!"));
		}
	}
}
