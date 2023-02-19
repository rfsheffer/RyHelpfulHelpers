// Copyright 2020-2023 Solar Storm Interactive

#include "RyRuntimeLogHelpers.h"
#include "RyRuntimeModule.h"

// Logging Includes
#include "Logging/LogMacros.h"
#include "Logging/LogCategory.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Console.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/FileHelper.h"
#include "GenericPlatform/GenericPlatformOutputDevices.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeLogHelpers::PrintLogString(UObject* WorldContextObject, const FString& InString, const FString& CategoryName, ERyRuntimeLogVerbosity verbosity,
                                          bool PrintToScreen, bool PrintToLog, FLinearColor TextColor /*= FColor::Blue*/, const float Duration /*= 0.0f*/)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) // Do not Print in Shipping or Test
    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    FString Prefix;
    if(World)
    {
        if(World->WorldType == EWorldType::PIE)
        {
            switch(World->GetNetMode())
            {
                case NM_Client:
                    Prefix = FString::Printf(TEXT("Client %d: "), GPlayInEditorID - 1);
                    break;
                case NM_DedicatedServer:
                case NM_ListenServer:
                    Prefix = FString::Printf(TEXT("Server: "));
                    break;
                case NM_Standalone:
                default:
                    break;
            }
        }
    }

    const FString FinalDisplayString = Prefix + InString;
#else
    const FString FinalDisplayString = InString;
#endif

    if(PrintToLog)
    {
        const ELogVerbosity::Type internalVerbosity = static_cast<ELogVerbosity::Type>(verbosity);
        FMsg::Logf_Internal(nullptr, 0, FLogCategoryName(*CategoryName), internalVerbosity, TEXT("%s"), *FinalDisplayString);
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) // Do not Print in Shipping or Test
        const APlayerController* PC = (WorldContextObject ? UGameplayStatics::GetPlayerController(WorldContextObject, 0) : nullptr);
        const ULocalPlayer* LocalPlayer = (PC ? Cast<ULocalPlayer>(PC->Player) : nullptr);
        if(LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->ViewportConsole)
        {
            LocalPlayer->ViewportClient->ViewportConsole->OutputText(FinalDisplayString);
        }
#endif
    }
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) // Do not Print in Shipping or Test
    if(PrintToScreen)
    {
        if(GAreScreenMessagesEnabled)
        {
            float actualDuration = Duration;
            if(GConfig && actualDuration < 0)
            {
                GConfig->GetFloat(TEXT("Kismet"), TEXT("PrintStringDuration"), actualDuration, GEngineIni);
            }
            GEngine->AddOnScreenDebugMessage(static_cast<uint64>(-1), actualDuration, TextColor.ToFColor(true), FinalDisplayString);
        }
        else
        {
            UE_LOG(LogRyRuntime, VeryVerbose, TEXT("Screen messages disabled (!GAreScreenMessagesEnabled).  Cannot print to screen."));
        }
    }
#endif // !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeLogHelpers::CopyCurrentLogFile(const FString& destLogFileName, FString& outLogFileName)
{
    FString path;
    FString fileName;
    FString ext;
    FPaths::Split(destLogFileName, path, fileName, ext);
    if(!IFileManager::Get().MakeDirectory(*path, true))
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("CopyCurrentLogFile: Unable to create directory for path '%s'"), *path);
        return false;
    }

    if(ext.IsEmpty())
    {
        ext = TEXT("log");
    }

    const FString filePathToTest = FPaths::Combine(path, fileName);
    outLogFileName = FString::Printf(TEXT("%s.%s"), *filePathToTest, *ext);
    if(FPaths::FileExists(outLogFileName))
    {
        FFileHelper::GenerateNextBitmapFilename(filePathToTest, ext, outLogFileName);
    }

    // Flush out the log
    GLog->Flush();

    if(!FGenericPlatformOutputDevices::GetLog()->IsMemoryOnly())
    {
        const FString logSrcAbsolute = FGenericPlatformOutputDevices::GetAbsoluteLogFilename();

        // Copy log
        return IFileManager::Get().Copy(*outLogFileName, *logSrcAbsolute,
                                        true, false, false, nullptr,
                                        FILEREAD_AllowWrite, FILEWRITE_AllowRead) == COPY_OK;
    }

    IFileManager& FileManager = IFileManager::Get();
    FArchive* ManifestFileArchive(FileManager.CreateFileWriter(*outLogFileName));
    if (ManifestFileArchive)
    {
        FGenericPlatformOutputDevices::GetLog()->Dump(*ManifestFileArchive);
        return true;
    }

    return false;
}
