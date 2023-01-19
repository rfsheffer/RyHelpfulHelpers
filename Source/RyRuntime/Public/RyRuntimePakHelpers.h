// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyRuntimePakHelpers.generated.h"

/**
 * Runtime helpers for working with Unreal Pak files
 */
UCLASS()
class RYRUNTIME_API URyRuntimePakHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Get a list of all pak files which have been successfully mounted
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PakHelpers")
	static void GetMountedPakFilenames(TArray<FString>& mountedPakFilenames);
	
	/**
	 * Request a pak file be mounted
	 * NOTE: Loading Paks should be reserved for package builds of your project, not editor builds.
	 * NOTE: If your project requires Pak signing, the paks you mount here must also be signed.
	 * NOTE: Once you have mounted your pak file you will have to map its internal pathing to a mounting point
	 *		 which you can do using the RegisterMountPoint node.
	 *		 You can discover pathing information about a Pak using UnrealPak.exe found in /UE_4.##/Engine/Binaries/Win64
	 *		 using the -List param. ex:
	 *		 UE_4.22/Engine/Binaries/Win64/UnrealPak.exe D:/MyPakFile.pak -List
	 * Once you have mounted a Pak and set the mounting point you can use nodes like LoadAsset and LoadObject to fetch assets
	 * from within your pak file using your mount point + path to your asset. ex: LoadObject("/MyMountPoint/MyAsset.MyAsset")
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PakHelpers")
	static bool MountPakFile(const FString& pakFilePath);

	/**
	 * Request a pak file be unmounted
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PakHelpers")
	static bool UnmountPakFile(const FString& pakFilePath);
};
