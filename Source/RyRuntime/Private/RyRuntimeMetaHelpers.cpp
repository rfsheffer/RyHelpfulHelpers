// Copyright 2020-2023 Solar Storm Interactive


#include "RyRuntimeMetaHelpers.h"
#include "Engine/UserDefinedEnum.h"
#include "Runtime/Launch/Resources/Version.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeMetaHelpers::GetEnumIndexFromName(const FString enumName, FName entryName, uint8& indexOut)
{
	// First try to find a static enum
	UObject* outerIn = nullptr;
#if ENGINE_MAJOR_VERSION < 5
	outerIn = ANY_PACKAGE;
#endif
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 7) || ENGINE_MAJOR_VERSION > 5
	const UEnum* EnumPtr = FindObject<UEnum>(outerIn, *enumName);
#else
	const UEnum* EnumPtr = FindObject<UEnum>(outerIn, *enumName, false);
#endif
	if(!EnumPtr)
	{
		// Look for the dynamic enum
		UUserDefinedEnum* userDefined = Cast<UUserDefinedEnum>(StaticLoadObject(UUserDefinedEnum::StaticClass(), nullptr, *enumName));
		if(userDefined)
		{
			const FString strEntryName = entryName.ToString();
			// User enums have generated names under the hood, the name given by the user is a localized text we need to
			// correspond with what they are looking for.
			for(int32 enumIndex = 0; enumIndex < userDefined->NumEnums(); ++enumIndex)
			{
				if(strEntryName == userDefined->GetAuthoredNameStringByIndex(enumIndex))
				{
					indexOut = enumIndex;
					return true;
				}
			}
		}
	}
	else
	{
		const int32 enumIndex = EnumPtr->GetIndexByName(entryName);
		if(enumIndex != INDEX_NONE)
		{
			indexOut = static_cast<uint8>(enumIndex);
			return true;
		}
	}
    
	return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeMetaHelpers::GetEnumValueFromName(const FString enumName, const FName entryName, int64& valueOut)
{
	UObject* outerIn = nullptr;
#if ENGINE_MAJOR_VERSION < 5
	outerIn = ANY_PACKAGE;
#endif
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 7) || ENGINE_MAJOR_VERSION > 5
	const UEnum* EnumPtr = FindObject<UEnum>(outerIn, *enumName);
#else
	const UEnum* EnumPtr = FindObject<UEnum>(outerIn, *enumName, false);
#endif
	if(!EnumPtr)
	{
		// Look for the dynamic enum
		UUserDefinedEnum* userDefined = Cast<UUserDefinedEnum>(StaticLoadObject(UUserDefinedEnum::StaticClass(), nullptr, *enumName));
		if(userDefined)
		{
			const FString strEntryName = entryName.ToString();
			// User enums have generated names under the hood, the name given by the user is a localized text we need to
			// correspond with what they are looking for.
			for(int32 enumIndex = 0; enumIndex < userDefined->NumEnums(); ++enumIndex)
			{
				if(strEntryName == userDefined->GetAuthoredNameStringByIndex(enumIndex))
				{
					valueOut = enumIndex;
					return true;
				}
			}
		}
	}
	else
	{
		valueOut = EnumPtr->GetValueByName(entryName);
		if(valueOut != INDEX_NONE)
		{
			return true;
		}
	}
    
	return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeMetaHelpers::GetEnumNameByIndex(const FString enumName, const uint8 enumIndex, FName& nameOut)
{
	UObject* outerIn = nullptr;
#if ENGINE_MAJOR_VERSION < 5
	outerIn = ANY_PACKAGE;
#endif
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 7) || ENGINE_MAJOR_VERSION > 5
	const UEnum* EnumPtr = FindObject<UEnum>(outerIn, *enumName);
#else
	const UEnum* EnumPtr = FindObject<UEnum>(outerIn, *enumName, false);
#endif
	if(!EnumPtr)
	{
		// Look for the dynamic enum
		UUserDefinedEnum* userDefined = Cast<UUserDefinedEnum>(StaticLoadObject(UUserDefinedEnum::StaticClass(), nullptr, *enumName));
		if(userDefined)
		{
			const FString nameFound = userDefined->GetAuthoredNameStringByIndex(enumIndex);
			if(!nameFound.IsEmpty())
			{
				nameOut = *nameFound;
				return true;
			}
		}
	}
	else
	{
		nameOut = EnumPtr->GetNameByIndex(enumIndex);
		if(nameOut != NAME_None)
		{
			return true;
		}
	}
    
	return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeMetaHelpers::GetEnumNameByValue(const FString enumName, const int64 enumValue, FName& nameOut)
{
	UObject* outerIn = nullptr;
#if ENGINE_MAJOR_VERSION < 5
	outerIn = ANY_PACKAGE;
#endif
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 7) || ENGINE_MAJOR_VERSION > 5
	const UEnum* EnumPtr = FindObject<UEnum>(outerIn, *enumName);
#else
	const UEnum* EnumPtr = FindObject<UEnum>(outerIn, *enumName, false);
#endif
	if(!EnumPtr)
	{
		// Look for the dynamic enum
		UUserDefinedEnum* userDefined = Cast<UUserDefinedEnum>(StaticLoadObject(UUserDefinedEnum::StaticClass(), nullptr, *enumName));
		if(userDefined)
		{
			const FString nameFound = userDefined->GetAuthoredNameStringByIndex(static_cast<int32>(enumValue));
			if(!nameFound.IsEmpty())
			{
				nameOut = *nameFound;
				return true;
			}
		}
	}
	else
	{
		nameOut = EnumPtr->GetNameByValue(enumValue);
		if(nameOut != NAME_None)
		{
			return true;
		}
	}
    
	return false;
}
