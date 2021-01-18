// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeStringHelpers.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeStringHelpers::IsNone(const FName name)
{
    return name.IsNone();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeStringHelpers::IsEmpty(const FString& string)
{
    return string.IsEmpty();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeStringHelpers::SetChar(FString& sourceString, const int32 charIndex, const FString& charIn)
{
    if(sourceString.Len() <= charIndex || charIn.Len() == 0 || charIndex < 0)
    {
        return;
    }

    TArray<TCHAR>& chars = sourceString.GetCharArray();
    chars[charIndex] = **charIn;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeStringHelpers::FillString(FString& sourceString, const int32 numChars, const FString& charIn)
{
    if(charIn.Len() == 0 || numChars <= 0)
    {
        return;
    }

    const TCHAR c = **charIn;
    TArray<TCHAR>& chars = sourceString.GetCharArray();
    chars.SetNumUninitialized(numChars + 1);
    for(int32 cIndex = 0; cIndex < numChars; ++cIndex)
    {
        chars[cIndex] = c;
    }
    chars[numChars] = 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeStringHelpers::SetReserved(FString& sourceString, const int32 reserveNumChars)
{
    if(reserveNumChars > 0)
    {
        sourceString.Reserve(reserveNumChars);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeStringHelpers::AppendTo(FString& sourceString, const FString& appendage)
{
    sourceString.Reserve(sourceString.Len() + appendage.Len());
    sourceString += appendage;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeStringHelpers::AppendStringsTo(FString& sourceString, const TArray<FString>& appendages)
{
    int32 numNewChars = 0;
    for(int32 appendageIndex = 0; appendageIndex < appendages.Num(); ++appendageIndex)
    {
        numNewChars += appendages[appendageIndex].Len();
    }

    sourceString.Reserve(sourceString.Len() + numNewChars);

    for(int32 appendageIndex = 0; appendageIndex < appendages.Num(); ++appendageIndex)
    {
        sourceString += appendages[appendageIndex];
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeStringHelpers::CharToBytes(const FString& sourceString, const int32 charIndex, uint8& byte1, uint8& byte2)
{
    if(sourceString.Len() <= charIndex || charIndex < 0)
    {
        byte1 = 0;
        byte2 = 0;
        return;
    }

    const TArray<TCHAR>& chars = sourceString.GetCharArray();
    const int32 cSize = sizeof(TCHAR);

    if(cSize > 1)
    {
#if PLATFORM_LITTLE_ENDIAN
        byte1 = ((uint8*)(&chars[charIndex]))[1];
        byte2 = ((uint8*)(&chars[charIndex]))[0];
#else
        byte1 = ((uint8*)(&chars[charIndex]))[0];
        byte2 = ((uint8*)(&chars[charIndex]))[1];
#endif
    }
    else
    {
        byte1 = 0;
        byte2 = ((uint8*)(&chars[charIndex]))[0];
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeStringHelpers::ToTitleStringInline(FString& inOutString)
{
    bool makeUpper = true;
    const int32 stringLength = inOutString.Len();
    TArray<TCHAR>& rawData = inOutString.GetCharArray();
    for (int32 i = 0; i < stringLength; ++i)
    {
        if(FChar::IsWhitespace(rawData[i]))
        {
            makeUpper = true;
            continue;
        }

        if(makeUpper)
        {
            rawData[i] = FChar::ToUpper(rawData[i]);
            makeUpper = false;
        }
        else
        {
            rawData[i] = FChar::ToLower(rawData[i]);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeStringHelpers::ToTitleString(const FString& inString)
{
    FString titleStrOut = inString;
    ToTitleStringInline(titleStrOut);
    return titleStrOut;
}
