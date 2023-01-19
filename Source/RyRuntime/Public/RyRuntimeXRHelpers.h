// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/HeadMountedDisplay/Public/IMotionController.h"
#include "Runtime/HeadMountedDisplay/Public/HeadMountedDisplayTypes.h"

#include "RyRuntimeXRHelpers.generated.h"

/** VR controller button and axis IDs */
UENUM(BlueprintType)
namespace ERySteamVRButtonIds
{
	enum Type
	{
		Button_System = 0,
        Button_ApplicationMenu = 1,
        Button_Grip = 2,
        Button_DPad_Left = 3,
        Button_DPad_Up = 4,
        Button_DPad_Right = 5,
        Button_DPad_Down = 6,

		// Button A for SteamVR native controllers and Oculus controllers
        Button_A = 7,

        Button_ProximitySensor = 31,

        Button_Axis0 = 32,
        Button_Axis1 = 33,
        Button_Axis2 = 34,
        Button_Axis3 = 35,
        Button_Axis4 = 36,

        Button_Max = 64
    };
}

//---------------------------------------------------------------------------------------------------------------------
/**
* Static Helper functions related to XR
*/
UCLASS()
class RYRUNTIME_API URyRuntimeXRHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	//UFUNCTION(BlueprintPure, Category = "RyRuntime|XR", DisplayName = "Get HMD Model Number")
	//static FString GetHMDModelNumber();
	
	UFUNCTION(BlueprintPure, Category = "RyRuntime|XR", DisplayName = "Get HMD Monitor Name")
	static FString GetHMDMonitorName();

	/**
	* Reports all devices currently available to the system, optionally limiting the result to a given class of devices.
	*
	* @param OutDevices The device ids of available devices will be appended to this array.
	* @param Type Optionally limit the list of devices to a certain type.
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|XR")
	static bool EnumerateTrackedDevices(TArray<int32>& OutDevices, EXRTrackedDeviceType Type = EXRTrackedDeviceType::Any);

	/**
	* Get the count of tracked devices
	* @param Type Optionally limit the count to a certain type
	* @return the count of matching tracked devices
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|XR")
	static int32 CountTrackedDevices(EXRTrackedDeviceType Type = EXRTrackedDeviceType::Any);

	/**
	* Returns the device type names of the controllers.
	* Use this function to identify the device order and what is assigned. Use the index of the device you care about
	* to call the other functions of XRHelpers.
	*
	* @return	Device type names of the controllers.
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|XR")
    static void GetMotionControllerDeviceTypeNames(TArray<FName>& namesOut);

	/**
	* Returns the tracking status (e.g. not tracked, intertial-only, fully tracked) of the specified controller
	*
	* @param deviceIndex - The IMotionController device index
	* @return	Tracking status of the specified controller, or ETrackingStatus::NotTracked if the device is not found
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|XR")
	static ETrackingStatus GetControllerTrackingStatus(const int32 deviceIndex, const int32 controllerIndex, const FName motionSource);

	/**
	* Called to request the motion sources that the deviceIndex IMotionController provides
	*
	* @param deviceIndex - The IMotionController device index
	* @param sourceNamesOut - The source names associated with this IMotionController device index
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|XR")
	static void EnumerateSources(const int32 deviceIndex, TArray<FName>& sourceNamesOut);

	/**
	* Returns a custom names parameter value
	*
	* @param MotionSource		The name of the motion source we want parameters for
	* @param ParameterName		The specific value we are looking for
	* @param bOutValueFound	(out) Whether the parameter could be found
	*
	* @return			The value of the parameter
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|XR")
	static float GetCustomParameterValue(const int32 deviceIndex, const FName MotionSource, FName ParameterName, bool& bOutValueFound);

	// Returns true if the position was retrieved.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|XR")
	static bool GetHandJointPosition(const int32 deviceIndex, const FName MotionSource, int jointIndex, FVector& OutPosition);
};
