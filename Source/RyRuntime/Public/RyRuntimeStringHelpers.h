// Copyright 2020-2021 Sheffer Online Services.
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

    /// Returns a string where the first character in every word is upper case. Like a header, or a title.
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|String")
    static void ToTitleStringInline(UPARAM(ref) FString& inOutString);

    /// Returns a string where the first character in every word is upper case. Like a header, or a title.
    UFUNCTION(BlueprintPure, Category = "RyRuntime|String")
    static FString ToTitleString(const FString& inString);

    /// Similar to MakeTextFromStringTable but instead of whining about a missing entry, sets isValid to false if no string exists or true if one does, and returns the text
    /// which is empty if there is no entry, or populated if there is.
    /// Also, takes in a string table asset instead of a table ID.
    /// @param table     The table to look in
    /// @param key       The key into the string table to find the text
    /// @param isValid   True if the text was found and set to the return of the function
    /// @return The text if it was found
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|Text", meta=(DisplayName="Get Text From String Table (Advanced)"))
    static FText GetTextFromStringTable(class UStringTable* table, const FString& key, bool& isValid);

    /// Similar to MakeTextFromStringTable but instead of whining about a missing entry, sets isValid to false if no string exists or true if one does, and returns the text
    /// which is empty if there is no entry, or populated if there is.
    /// Try to use GetTextFromStringTable instead as it should help have the asset pre-loaded. If you call GetTextFromStringTableID
    /// while the string table isn't loaded, it will try to syncronously load the string table asset.
    /// @param tableID   The table to look up
    /// @param key       The key into the string table to find the text
    /// @param isValid   True if the text was found and set to the return of the function
    /// @return The text if it was found
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|Text", meta=(DisplayName="Get Text From String Table ID (Advanced)"))
    static FText GetTextFromStringTableID(FName tableID, const FString& key, bool& isValid);

    /** Converts a int64 value to a string */
    UFUNCTION(BlueprintPure, meta=(DisplayName = "ToString (Int64)", CompactNodeTitle = "->", BlueprintAutocast), Category="RyRuntime|String")
    static FString Conv_Int64ToString(int64 InInt64);

    /** Combines file paths */
    UFUNCTION(BlueprintPure, meta=(CommutativeAssociativeBinaryOperator = "true"), Category="RyRuntime|String|FilePaths")
    static FString CombineFilePaths(FString A, FString B);
};
