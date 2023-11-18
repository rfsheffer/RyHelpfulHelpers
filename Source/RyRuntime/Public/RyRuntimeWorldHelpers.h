﻿// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/LatentActionManager.h"
#include "RyRuntimeLevelHelpers.h"
#include "RyRuntimeRenderingHelpers.h"

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

USTRUCT(BlueprintType)
struct FRyWorldContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="RyWorldContext")
	UWorld* World = nullptr;

	UPROPERTY(BlueprintReadWrite, Category="RyWorldContext")
	ERyWorldType	WorldType = ERyWorldType::None;

	UPROPERTY(BlueprintReadWrite, Category="RyWorldContext")
	FName ContextHandle;

	/** URL to travel to for pending client connect */
	UPROPERTY(BlueprintReadWrite, Category="RyWorldContext")
	FString TravelURL;

	/** TravelType for pending client connects */
	UPROPERTY(BlueprintReadWrite, Category="RyWorldContext")
	uint8 TravelType = 0;
	
	UPROPERTY(BlueprintReadWrite, Category="RyWorldContext")
	class UGameInstance* OwningGameInstance = nullptr;

	/** The Prefix in front of PIE level names, empty is default */
	UPROPERTY(BlueprintReadWrite, Category="RyWorldContext")
	FString	PIEPrefix;

	/** Is this running as a dedicated server */
	UPROPERTY(BlueprintReadWrite, Category="RyWorldContext")
	bool	RunAsDedicated = false;

	/** Is this world context waiting for an online login to complete (for PIE) */
	UPROPERTY(BlueprintReadWrite, Category="RyWorldContext")
	bool	bWaitingOnOnlineSubsystem = false;

	/** Custom description to be display in blueprint debugger UI */
	UPROPERTY(BlueprintReadWrite, Category="RyWorldContext")
	FString CustomDescription;
};

/**
 * Helpers related to the world (actors, components, etc)
 */
UCLASS()
class RYRUNTIME_API URyRuntimeWorldHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
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


	/**
	 * Tries to return the active world following these rules:
	 * If WorldContextObject and specificType are both None:
	 * If in the editor and PIE is not active, this will be the editor world, else
	 * If in a game (PIE or otherwise) it will try to return that game world
	 * NOTE: Since there can be many worlds in Unreal Engine this call might be ambiguous
	 * @param WorldContextObject If non-None, will return the world associated with the object if possible, else returns None
	 * @param specificType The specific world type to try and get if set to non-None, else returns None
	 * @param gameWorldOnly If true, will never return an editor world if searching and specificType is not set
	 */
	UFUNCTION(BlueprintCallable, Category="RyRuntime|WorldHelpers")
	static UWorld* GetEngineWorld(UObject* WorldContextObject = nullptr, ERyWorldType specificType = ERyWorldType::None, bool gameWorldOnly = false);

	/**
	 * Get all current registered worlds with the engine and context info
	 */
	UFUNCTION(BlueprintCallable, Category="RyRuntime|WorldHelpers")
	static void GetEngineWorldContexts(TArray<FRyWorldContext>& worldsOut);

	/**
	 * Find a Local Player Controller, which may not exist at all if this is a server.
	 * Returns first found LocalPlayerController. Fine for single player, in split screen, one will be picked. 
	 * @param WorldContextObject Object to get the world of
	 * @return first found LocalPlayerController. Fine for single player, in split screen, one will be picked. 
	 */
	UFUNCTION(BlueprintPure, Category="RyRuntime|WorldHelpers", meta=(WorldContext="WorldContextObject"))
	static class APlayerController* GetFirstLocalPlayerController(UObject* WorldContextObject);

	/**
	 * Set a timer to execute delegate. Setting an existing timer will reset that timer with updated parameters.
	 * @param Event						Event. Can be a K2 function or a Custom Event.
	 * @param Time						How long to wait before executing the delegate, in seconds. Setting a timer to <= 0 seconds will clear it if it is set.
	 * @param bLooping					True to keep executing the delegate every Time seconds, false to execute delegate only once.
	 * @param InitialStartDelay			Initial delay passed to the timer manager, in seconds.
	 * @param InitialStartDelayVariance	Use this to add some variance to when the timer starts in lieu of doing a random range on the InitialStartDelay input, in seconds. 
	 * @return							The timer handle to pass to other timer functions to manipulate this timer.
	 */
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay="InitialStartDelay, InitialStartDelayVariance"), Category="RyRuntime|WorldHelpers")
	static FTimerHandle SetTimerByEventForWorld(UObject* WorldContextObject, UPARAM(DisplayName="Event") FTimerDynamicDelegate Delegate, float Time, bool bLooping, float InitialStartDelay = 0.f, float InitialStartDelayVariance = 0.f);

	/**
	 * Set a timer to execute delegate. Setting an existing timer will reset that timer with updated parameters.
	 * @param Object					Object that implements the delegate function. Defaults to self (this blueprint)
	 * @param FunctionName				Delegate function name. Can be a K2 function or a Custom Event.
	 * @param Time						How long to wait before executing the delegate, in seconds. Setting a timer to <= 0 seconds will clear it if it is set.
	 * @param bLooping					true to keep executing the delegate every Time seconds, false to execute delegate only once.
	 * @param InitialStartDelay			Initial delay passed to the timer manager to allow some variance in when the timer starts, in seconds.
	 * @param InitialStartDelayVariance	Use this to add some variance to when the timer starts in lieu of doing a random range on the InitialStartDelay input, in seconds.
	 * @return							The timer handle to pass to other timer functions to manipulate this timer.
	 */
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay="InitialStartDelay, InitialStartDelayVariance"), Category="RyRuntime|WorldHelpers")
	static FTimerHandle SetTimerByFunctionForWorld(UObject* WorldContextObject, UObject* Object, FString FunctionName, float Time, bool bLooping, float InitialStartDelay = 0.f, float InitialStartDelayVariance = 0.f);

	/**
	 * Clears a set timer.
	 * @param Handle		The handle of the timer to clear.
	 */
	UFUNCTION(BlueprintCallable, Category="RyRuntime|WorldHelpers")
	static void ClearAndInvalidateTimerHandleForWorld(const UObject* WorldContextObject, UPARAM(ref) FTimerHandle& Handle);

	/**
	 * Returns time in seconds since world was brought up for play, IS stopped when game pauses, IS dilated/clamped
	 *
	 * @return time in seconds since world was brought up for play
	 */
	UFUNCTION(BlueprintPure, Category="RyRuntime|WorldHelpers")
	static float GetTimeSecondsForWorld(const UObject* WorldContextObject);

	/**
	* Returns time in seconds since world was brought up for play, IS NOT stopped when game pauses, IS dilated/clamped
	*
	* @return time in seconds since world was brought up for play
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|WorldHelpers")
	static float GetUnpausedTimeSecondsForWorld(const UObject* WorldContextObject);
	
	/**
	* Returns time in seconds since world was brought up for play, does NOT stop when game pauses, NOT dilated/clamped
	*
	* @return time in seconds since world was brought up for play
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|WorldHelpers")
	static float GetRealTimeSecondsForWorld(const UObject* WorldContextObject);
};
