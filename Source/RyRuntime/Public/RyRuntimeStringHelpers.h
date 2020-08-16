// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyRuntimeStringHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions for string containers.
*/
UCLASS(meta=(BlueprintThreadSafe))
class RYRUNTIME_API URyRuntimeStringHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    /**
     * Is the name None?
    */
    UFUNCTION(BlueprintPure, Category = "RyRuntime|Name")
    static bool IsNone(const FName name);

    /**
     * Is the string empty? (Has no characters)
    */
    UFUNCTION(BlueprintPure, Category = "RyRuntime|String")
    static bool IsEmpty(const FString& string);

    /**
     * Sets a char by index in the string
    */
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|String")
    static void SetChar(UPARAM(ref) FString& sourceString, const int32 charIndex, const FString& charIn);

    /**
     * Fills a string with a specific character overwriting the original string. Good for initializing a string with characters to start.
    */
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|String")
    static void FillString(UPARAM(ref) FString& sourceString, const int32 numChars, const FString& charIn);

    /**
     * Sets the reserved number of characters in a string.
    */
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|String")
    static void SetReserved(UPARAM(ref) FString& sourceString, const int32 reserveNumChars);

    /**
     * Append a string to a source string
    */
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|String")
    static void AppendTo(UPARAM(ref) FString& sourceString, const FString& appendage);

    /**
     * Append many strings to a source string
    */
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|String")
    static void AppendStringsTo(UPARAM(ref) FString& sourceString, const TArray<FString>& appendages);

    /**
     * Returns the bytes representing a character in a string. Two bytes to represent a UTF-16 TCHAR.
     * If TCHAR is 8bits on this system, byte2 will contain the character, byte1 will be zero.
    */
    UFUNCTION(BlueprintPure, Category = "RyRuntime|String")
    static void CharToBytes(const FString& sourceString, const int32 charIndex, uint8& byte1, uint8& byte2);
};
