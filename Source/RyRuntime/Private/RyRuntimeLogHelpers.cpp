// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeLogHelpers.h"

// Logging Includes
#include "Logging/LogMacros.h"
#include "Logging/LogCategory.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Console.h"
#include "Misc/ConfigCacheIni.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeLogHelpers::PrintLogString(UObject* WorldContextObject, const FString& InString, const FString& CategoryName, ERyRuntimeLogVerbosity verbosity,
                                          bool PrintToScreen, bool PrintToLog, FLinearColor TextColor /*= FColor::Blue*/, const float Duration /*= 0.0f*/)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) // Do not Print in Shipping or Test
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
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
                    break;
            }
        }
    }

    const FString FinalDisplayString = Prefix + InString;

    if(PrintToLog)
    {
        ELogVerbosity::Type internalVerbosity = (ELogVerbosity::Type)verbosity;
        FMsg::Logf_Internal(nullptr, 0, FLogCategoryName(*CategoryName), internalVerbosity, TEXT("%s"), *FinalDisplayString);

        APlayerController* PC = (WorldContextObject ? UGameplayStatics::GetPlayerController(WorldContextObject, 0) : NULL);
        ULocalPlayer* LocalPlayer = (PC ? Cast<ULocalPlayer>(PC->Player) : NULL);
        if(LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->ViewportConsole)
        {
            LocalPlayer->ViewportClient->ViewportConsole->OutputText(FinalDisplayString);
        }
    }

    if(PrintToScreen)
    {
        if(GAreScreenMessagesEnabled)
        {
            float actualDuration = Duration;
            if(GConfig && actualDuration < 0)
            {
                GConfig->GetFloat(TEXT("Kismet"), TEXT("PrintStringDuration"), actualDuration, GEngineIni);
            }
            GEngine->AddOnScreenDebugMessage((uint64)-1, actualDuration, TextColor.ToFColor(true), FinalDisplayString);
        }
        else
        {
            UE_LOG(LogBlueprint, VeryVerbose, TEXT("Screen messages disabled (!GAreScreenMessagesEnabled).  Cannot print to screen."));
        }
    }
#endif // !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
}
