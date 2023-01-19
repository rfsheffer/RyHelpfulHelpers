// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyRuntimeMetaHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to Unreal Reflection Metadata
*/
UCLASS()
class RYRUNTIME_API URyRuntimeMetaHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/// Get the index of an enum entry in an enum
	/// @param enumName The name of the enum, ex: EMyEnumName. If this is a blueprint enum, a full path is required, ex: /Game/MyBlueprintEnum.
	/// @param entryName The entry in the name to find and return the index
	/// @param indexOut The index out
	/// @return True if found and indexOut is valid
	UFUNCTION(BlueprintPure, Category = "RyRuntime|MetaHelpers")
	static bool GetEnumIndexFromName(const FString enumName, FName entryName, uint8& indexOut);

	/// Get the value of an enum entry in an enum
	/// @param enumName The name of the enum, ex: EMyEnumName. If this is a blueprint enum, a full path is required, ex: /Game/MyBlueprintEnum.
	/// @param entryName The entry in the name to find and return the value
	/// @param valueOut The value out
	/// @return True if found and valueOut is valid
	UFUNCTION(BlueprintPure, Category = "RyRuntime|MetaHelpers")
	static bool GetEnumValueFromName(const FString enumName, const FName entryName, int64& valueOut);

	/// Get an enums entry name by enum index
	/// @param enumName The name of the enum, ex: EMyEnumName. If this is a blueprint enum, a full path is required, ex: /Game/MyBlueprintEnum.
	/// @param enumIndex The index to lookup
	/// @param nameOut The name out
	/// @return True if the name is valid (the index was valid)
	UFUNCTION(BlueprintPure, Category = "RyRuntime|MetaHelpers")
	static bool GetEnumNameByIndex(const FString enumName, const uint8 enumIndex, FName& nameOut);

	/// Get an enums entry name by enum value
	/// @param enumName The name of the enum, ex: EMyEnumName. If this is a blueprint enum, a full path is required, ex: /Game/MyBlueprintEnum.
	/// @param enumValue The value to lookup
	/// @param nameOut The name out
	/// @return True if the name is valid (the index was valid)
	UFUNCTION(BlueprintPure, Category = "RyRuntime|MetaHelpers")
	static bool GetEnumNameByValue(const FString enumName, const int64 enumValue, FName& nameOut);
};
