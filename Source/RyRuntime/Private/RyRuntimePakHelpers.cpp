// Copyright 2020-2023 Solar Storm Interactive


#include "RyRuntimePakHelpers.h"
#include "IPlatformFilePak.h"
#include "RyRuntimeModule.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimePakHelpers::GetMountedPakFilenames(TArray<FString>& mountedPakFilenames)
{
	mountedPakFilenames.Reset();
	FPakPlatformFile* pakFileMgr = static_cast<FPakPlatformFile*>(FPlatformFileManager::Get().GetPlatformFile(TEXT("PakFile")));
	if (!pakFileMgr || !pakFileMgr->GetLowerLevel())
	{
		return;
	}

	pakFileMgr->GetMountedPakFilenames(mountedPakFilenames);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimePakHelpers::MountPakFile(const FString& pakFilePath)
{
	if(!FPlatformProperties::RequiresCookedData())
	{
		UE_LOG(LogRyRuntime, Error, TEXT("MountPakFile failed because the current project state does not allow cooked content! You must be in an editor build. PlatformRequiresCookedData() should be true to call this function."));
		return false;
	}
	if (!FCoreDelegates::MountPak.IsBound())
	{
		UE_LOG(LogRyRuntime, Error, TEXT("MountPakFile failed because pak mounting has not been initialized by the engine!"));
		return false;
	}

	return FCoreDelegates::MountPak.Execute(pakFilePath, 0) != nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimePakHelpers::UnmountPakFile(const FString& pakFilePath)
{
	if(!FPlatformProperties::RequiresCookedData())
	{
		UE_LOG(LogRyRuntime, Error, TEXT("UnmountPakFile failed because the current project state does not allow cooked content! You must be in an editor build. PlatformRequiresCookedData() should be true to call this function."));
		return false;
	}
	if (!FCoreDelegates::OnUnmountPak.IsBound())
	{
		UE_LOG(LogRyRuntime, Error, TEXT("UnmountPakFile failed because pak mounting has not been initialized by the engine!"));
		return false;
	}
	
	return FCoreDelegates::OnUnmountPak.Execute(pakFilePath);
}
