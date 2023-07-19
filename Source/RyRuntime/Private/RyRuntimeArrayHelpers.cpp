// Copyright 2020-2023 Solar Storm Interactive

#include "RyRuntimeArrayHelpers.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeArrayHelpers::Array_GetLastItem(const TArray<int32>& TargetArray, int32& LastItem)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
	return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeArrayHelpers::GenericArray_GetLastItem(const void* TargetArray, const FArrayProperty* ArrayProp, void* LastItem)
{
	if( TargetArray )
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		if(ArrayHelper.Num() > 0)
		{
			FProperty* InnerProp = ArrayProp->Inner;
			InnerProp->CopyCompleteValueFromScriptVM(LastItem, ArrayHelper.GetRawPtr(ArrayHelper.Num() - 1));	
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeArrayHelpers::Array_Pop(const TArray<int32>& TargetArray, int32& Item)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
	return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeArrayHelpers::GenericArray_Pop(const void* TargetArray, const FArrayProperty* ArrayProp, void* PoppedItem)
{
	if( TargetArray )
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		if(ArrayHelper.Num() > 0)
		{
			FProperty* InnerProp = ArrayProp->Inner;
			InnerProp->CopyCompleteValueFromScriptVM(PoppedItem, ArrayHelper.GetRawPtr(ArrayHelper.Num() - 1));
			ArrayHelper.RemoveValues(ArrayHelper.Num() - 1, 1);
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeArrayHelpers::Array_IsEmpty(const TArray<int32>& TargetArray)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
	return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeArrayHelpers::GenericArray_IsEmpty(const void* TargetArray, const FArrayProperty* ArrayProp)
{
	if( TargetArray )
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		return ArrayHelper.Num() == 0;
	}

	return true;
}

//---------------------------------------------------------------------------------------------------------------------
/**

void URyRuntimeArrayHelpers::Array_Sort(const TArray<int32>& TargetArray, FRyGenericArraySort SortFunc)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
}
	
void URyRuntimeArrayHelpers::GenericArray_Sort(const void* TargetArray, const FArrayProperty* ArrayProp, const void* DelegateAddr, const FDelegateProperty* DelegateProperty)
{
	
}*/
