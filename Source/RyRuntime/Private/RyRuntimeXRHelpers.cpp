// Copyright 2020-2022 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeXRHelpers.h"
#include "RyRuntimeModule.h"

#include "IHeadMountedDisplay.h"
#include "Runtime/Core/Public/Features/IModularFeatures.h"
#include "Runtime/HeadMountedDisplay/Public/IXRTrackingSystem.h"

#define STEAMVR_SUPPORTED_PLATFORM (PLATFORM_MAC || (PLATFORM_LINUX && PLATFORM_CPU_X86_FAMILY && PLATFORM_64BITS) || (PLATFORM_WINDOWS && WINVER > 0x0502))
#define OCULUS_HMD_SUPPORTED_PLATFORM (PLATFORM_WINDOWS && WINVER > 0x0502) || (PLATFORM_ANDROID_ARM || PLATFORM_ANDROID_ARM64)

/*#if STEAMVR_SUPPORTED_PLATFORM
#include "ISteamVRPlugin.h"
#endif


#if OCULUS_HMD_SUPPORTED_PLATFORM
#include "IOculusHMDModule.h"
#endif
*/

//---------------------------------------------------------------------------------------------------------------------
/**

FString URyRuntimeXRHelpers::GetHMDModelNumber()
{
#if STEAMVR_SUPPORTED_PLATFORM
	if(ISteamVRPlugin::IsAvailable() && ISteamVRPlugin::Get().GetVRSystem() && ISteamVRPlugin::Get().IsHMDConnected())
	{
		vr::HmdError HmdErr;
		vr::IVRSystem *pVRSystem = static_cast<vr::IVRSystem*>(vr::VR_GetGenericInterface(vr::IVRSystem_Version, &HmdErr));
		if(pVRSystem)
		{
			for(vr::TrackedDeviceIndex_t deviceIndex = 0; deviceIndex < vr::k_unMaxTrackedDeviceCount; ++deviceIndex)
			{
				if(pVRSystem->IsTrackedDeviceConnected(deviceIndex) &&
                   pVRSystem->GetTrackedDeviceClass(deviceIndex) == vr::ETrackedDeviceClass::TrackedDeviceClass_HMD)
				{
					char chValue[1024];
					vr::ETrackedPropertyError trackPropErr;
					pVRSystem->GetStringTrackedDeviceProperty(deviceIndex,
                                                              vr::ETrackedDeviceProperty::Prop_ModelNumber_String,
                                                              chValue, 1024, &trackPropErr);
					if(trackPropErr == vr::TrackedProp_Success)
					{
						return ANSI_TO_TCHAR(chValue);
					}
				}
			}
		}
	}
#endif
#if OCULUS_HMD_SUPPORTED_PLATFORM
	if(IOculusHMDModule::IsAvailable() && IOculusHMDModule::Get().IsHMDConnected())
	{
		// This isn't great...
		return TEXT("OculusHMD");
	}
#endif
	
	return TEXT("Unknown");
}*/

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeXRHelpers::GetHMDMonitorName()
{
	if (GEngine->XRSystem.IsValid())
	{
		IXRTrackingSystem* XRSystem = GEngine->XRSystem.Get();
		IHeadMountedDisplay* display = XRSystem->GetHMDDevice();
		if(display)
		{
			IHeadMountedDisplay::MonitorInfo monitor;
			if(display->GetHMDMonitorInfo(monitor))
			{
				return monitor.MonitorName;
			}
		}
	}
	return TEXT("Unknown");
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeXRHelpers::EnumerateTrackedDevices(TArray<int32>& OutDevices, EXRTrackedDeviceType Type)
{
	if (GEngine->XRSystem.IsValid())
	{
		IXRTrackingSystem* XRSystem = GEngine->XRSystem.Get();
		return XRSystem->EnumerateTrackedDevices(OutDevices, Type);
	}
	return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
int32 URyRuntimeXRHelpers::CountTrackedDevices(EXRTrackedDeviceType Type)
{
	if (GEngine->XRSystem.IsValid())
	{
		IXRTrackingSystem* XRSystem = GEngine->XRSystem.Get();
		return XRSystem->CountTrackedDevices(Type);
	}
	return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeXRHelpers::GetMotionControllerDeviceTypeNames(TArray<FName>& namesOut)
{
	TArray<IMotionController*> motionControllers = IModularFeatures::Get().GetModularFeatureImplementations<IMotionController>(IMotionController::GetModularFeatureName());
	for (IMotionController* motionController : motionControllers)
	{
		namesOut.Add(motionController->GetMotionControllerDeviceTypeName());
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ETrackingStatus URyRuntimeXRHelpers::GetControllerTrackingStatus(const int32 deviceIndex, const int32 controllerIndex, const FName motionSource)
{
	TArray<IMotionController*> motionControllers = IModularFeatures::Get().GetModularFeatureImplementations<IMotionController>(IMotionController::GetModularFeatureName());
	if(motionControllers.IsValidIndex(deviceIndex))
	{
		return motionControllers[deviceIndex]->GetControllerTrackingStatus(controllerIndex, motionSource);
	}

	return ETrackingStatus::NotTracked;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeXRHelpers::EnumerateSources(const int32 deviceIndex, TArray<FName>& sourceNamesOut)
{
	TArray<IMotionController*> motionControllers = IModularFeatures::Get().GetModularFeatureImplementations<IMotionController>(IMotionController::GetModularFeatureName());
	if(motionControllers.IsValidIndex(deviceIndex))
	{
		TArray<FMotionControllerSource> sourcesOut;
		motionControllers[deviceIndex]->EnumerateSources(sourcesOut);
		for(const FMotionControllerSource& source : sourcesOut)
		{
			sourceNamesOut.Add(source.SourceName);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
float URyRuntimeXRHelpers::GetCustomParameterValue(const int32 deviceIndex, const FName MotionSource, FName ParameterName, bool& bOutValueFound)
{
	TArray<IMotionController*> motionControllers = IModularFeatures::Get().GetModularFeatureImplementations<IMotionController>(IMotionController::GetModularFeatureName());
	if(motionControllers.IsValidIndex(deviceIndex))
	{
		return motionControllers[deviceIndex]->GetCustomParameterValue(MotionSource, ParameterName, bOutValueFound);
	}

	bOutValueFound = false;
	return 0.0f;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeXRHelpers::GetHandJointPosition(const int32 deviceIndex, const FName MotionSource, int jointIndex, FVector& OutPosition)
{
	TArray<IMotionController*> motionControllers = IModularFeatures::Get().GetModularFeatureImplementations<IMotionController>(IMotionController::GetModularFeatureName());
	if(motionControllers.IsValidIndex(deviceIndex))
	{
		return motionControllers[deviceIndex]->GetHandJointPosition(MotionSource, jointIndex, OutPosition);
	}

	return false;
}
