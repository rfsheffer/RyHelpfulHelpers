// Copyright 2020-2022 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/LatentActionManager.h"

#include "RyRuntimeWorldHelpers.generated.h"

UENUM(BlueprintType)
enum class ERyTickUntilStopInExecs : uint8
{
	Start,
	Stop
};

UENUM(BlueprintType)
enum class ERyTickUntilStopOutExecs : uint8
{
	Tick,
	Stopped
};

/**
 * Helpers related to the world (actors, components, etc)
 */
UCLASS()
class RYRUNTIME_API URyRuntimeWorldHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/** 
	* Executes the "Tick" pin every tick until the "Stop" pin is executed. Calling "Start" again while it is still ticking will be ignored.
	* 
	* @param WorldContextObject		The world context
	* @param InExecs				Input (Start or Stop)
	* @param OutExecs				Output (Stopped or Tick)
	* @param LatentInfo				The persistent latent info
	*/
	UFUNCTION(BlueprintCallable, Category="RyRuntime|WorldHelpers|FlowControl", meta=(Latent = "", WorldContext="WorldContextObject", LatentInfo="LatentInfo", ExpandEnumAsExecs="InExecs,OutExecs"))
	static void TickUntilStop(const UObject* WorldContextObject, const ERyTickUntilStopInExecs& InExecs, ERyTickUntilStopOutExecs& OutExecs, FLatentActionInfo LatentInfo);
};
