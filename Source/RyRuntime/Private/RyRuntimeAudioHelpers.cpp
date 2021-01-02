// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeAudioHelpers.h"
#include "RyRuntimeModule.h"
#include "Runtime/Engine/Public/AudioDevice.h"

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

#if PLATFORM_WINDOWS
//---------------------------------------------------------------------------------------------------------------------
/**
*/
/** Callback to access all the voice capture devices on the platform */
BOOL CALLBACK RyCaptureDeviceCallback(
    LPGUID lpGuid,
    LPCSTR lpcstrDescription,
    LPCSTR lpcstrModule,
    LPVOID lpContext
    )
{
	if (lpGuid != nullptr)
	{
		// Save the enumerated device information for later use
		const FString DeviceDescription((LPCWSTR)lpcstrDescription);
		URyRuntimeAudioHelpers* defaultObj = GetMutableDefault<URyRuntimeAudioHelpers>();
		if(!defaultObj)
		{
			return true;
		}

		defaultObj->CachedCaptureDeviceList.Add(DeviceDescription);
	}
	
	return true;
}
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeAudioHelpers::GetAudioCaptureDeviceList(TArray<FString>& OutAudioCaptureDeviceNames)
{
#if PLATFORM_WINDOWS
	URyRuntimeAudioHelpers* defaultObj = GetMutableDefault<URyRuntimeAudioHelpers>();
	if(!defaultObj)
	{
		return;
	}

	defaultObj->CachedCaptureDeviceList.Empty();

	HRESULT hr = DirectSoundCaptureEnumerate((LPDSENUMCALLBACK)RyCaptureDeviceCallback, nullptr);
	if (FAILED(hr))
	{
		UE_LOG(LogRyRuntime, Warning, TEXT("Failed to enumerate capture devices %d"), hr);
		return;
	}

	OutAudioCaptureDeviceNames = defaultObj->CachedCaptureDeviceList;
	
#endif
}
