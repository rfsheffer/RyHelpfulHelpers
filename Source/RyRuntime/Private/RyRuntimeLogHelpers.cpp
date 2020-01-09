// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#include "RyRuntimeLogHelpers.h"
#include "Logging/LogMacros.h"
#include "Logging/LogCategory.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeLogHelpers::PrintLogString(const FString& InString, const FString& CategoryName, ERyRuntimeLogVerbosity verbosity,
                                          bool PrintToScreen, bool PrintToLog, FLinearColor TextColor /*= FColor::Blue*/, const float Duration /*= 0.0f*/)
{
    if(PrintToLog)
    {
        ELogVerbosity::Type internalVerbosity = (ELogVerbosity::Type)verbosity;
        FMsg::Logf_Internal(nullptr, 0, FLogCategoryName(*CategoryName), internalVerbosity, TEXT("%s"), *InString);
    }

    if(PrintToScreen && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, Duration, TextColor.ToFColor(true), InString);
    }
}
