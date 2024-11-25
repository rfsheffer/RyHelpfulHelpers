// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Launch/Resources/Version.h"

#include "RyRuntimeArrayHelpers.generated.h"

//DECLARE_DYNAMIC_DELEGATE_RetVal_TwoParams(bool, FRyGenericArraySort, const int32&, A, const int32&, B);

/**
 * Helpers for generic arrays
 */
UCLASS()
class RYRUNTIME_API URyRuntimeArrayHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/* 
	 * Get the last item in this array.
	 *
	 *@param	TargetArray		The array to get the last item
	 *@param    LastItem        The last item in the array
	 *@return	True if an item was retrieved. False if there are no items in the array and LastItem is defaulted.
	*/
	UFUNCTION(BlueprintPure, CustomThunk, meta=(DisplayName = "Get Last Item", CompactNodeTitle = "LAST", ArrayParm = "TargetArray", Keywords = "last index tail", ArrayTypeDependentParams = "LastItem", BlueprintThreadSafe), Category="RyRuntime|Array")
	static bool Array_GetLastItem(const TArray<int32>& TargetArray, int32& LastItem);

	static bool GenericArray_GetLastItem(const void* TargetArray, const FArrayProperty* ArrayProp, void* LastItem);
	DECLARE_FUNCTION(execArray_GetLastItem)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		// Since 'Item' isn't really an int, step the stack manually
		// We use the array inner type to understand what we are returning (the size)
		const FProperty* InnerProp = ArrayProperty->Inner;
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5) || ENGINE_MAJOR_VERSION > 5
		const int32 PropertySize = InnerProp->GetElementSize() * InnerProp->ArrayDim;
#else
		const int32 PropertySize = InnerProp->ElementSize * InnerProp->ArrayDim;
#endif
		void* StorageSpace = FMemory_Alloca(PropertySize);
		InnerProp->InitializeValue(StorageSpace);

		Stack.MostRecentPropertyAddress = NULL;
		Stack.StepCompiledIn<FProperty>(StorageSpace);
		void* ItemPtr = (Stack.MostRecentPropertyAddress != NULL && Stack.MostRecentProperty->GetClass() == InnerProp->GetClass()) ? Stack.MostRecentPropertyAddress : StorageSpace;

		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)RESULT_PARAM = GenericArray_GetLastItem(ArrayAddr, ArrayProperty, ItemPtr);
		P_NATIVE_END;
		InnerProp->DestroyValue(StorageSpace);
	}

	/* 
	 * Pop an item off the end of the array and return it
	 *
	 *@param	TargetArray		The array to pop an element off the end
	 *@param	Item		The item popped
	 *@return	True if the array had an element to pop and Item is valid.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, meta=(DisplayName = "Pop", CompactNodeTitle = "POP", ArrayParm = "TargetArray", ArrayTypeDependentParams = "Item", Keywords = "pop"), Category="RyRuntime|Array")
	static bool Array_Pop(const TArray<int32>& TargetArray, int32& Item);

	static bool GenericArray_Pop(const void* TargetArray, const FArrayProperty* ArrayProp, void* PoppedItem);
	DECLARE_FUNCTION(execArray_Pop)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		// Since 'Item' isn't really an int, step the stack manually
		// We use the array inner type to understand what we are returning (the size)
		const FProperty* InnerProp = ArrayProperty->Inner;
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5) || ENGINE_MAJOR_VERSION > 5
		const int32 PropertySize = InnerProp->GetElementSize() * InnerProp->ArrayDim;
#else
		const int32 PropertySize = InnerProp->ElementSize * InnerProp->ArrayDim;
#endif
		void* StorageSpace = FMemory_Alloca(PropertySize);
		InnerProp->InitializeValue(StorageSpace);

		Stack.MostRecentPropertyAddress = NULL;
		Stack.StepCompiledIn<FProperty>(StorageSpace);
		void* ItemPtr = (Stack.MostRecentPropertyAddress != NULL && Stack.MostRecentProperty->GetClass() == InnerProp->GetClass()) ? Stack.MostRecentPropertyAddress : StorageSpace;

		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)RESULT_PARAM = GenericArray_Pop(ArrayAddr, ArrayProperty, ItemPtr);
		P_NATIVE_END;
		InnerProp->DestroyValue(StorageSpace);
	}

	/* 
	 * Returns true if the array is empty
	 *
	 *@param	TargetArray		The array to check for empty
	 *@return	True if the array is empty
	*/
	UFUNCTION(BlueprintPure, CustomThunk, meta=(DisplayName = "Is Empty", CompactNodeTitle = "IS EMPTY", ArrayParm = "TargetArray", Keywords = "empty", BlueprintThreadSafe), Category="RyRuntime|Array")
	static bool Array_IsEmpty(const TArray<int32>& TargetArray);

	static bool GenericArray_IsEmpty(const void* TargetArray, const FArrayProperty* ArrayProp);
	DECLARE_FUNCTION(execArray_IsEmpty)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)RESULT_PARAM = GenericArray_IsEmpty(ArrayAddr, ArrayProperty);
		P_NATIVE_END;
	}

	/*UFUNCTION(BlueprintCallable, CustomThunk, meta=(DisplayName = "Sort", CompactNodeTitle = "SORT", ArrayParm = "TargetArray", ArrayTypeDependentParams = "SortFunc", Keywords = "sort"), Category="RyRuntime|Array")
	static void Array_Sort(const TArray<int32>& TargetArray, FRyGenericArraySort SortFunc);

	static void GenericArray_Sort(const void* TargetArray, const FArrayProperty* ArrayProp, const void* DelegateAddr, const FDelegateProperty* DelegateProperty);
	DECLARE_FUNCTION(execArray_Sort)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}
		
		Stack.StepCompiledIn<FDelegateProperty>(NULL);
		void* DelegateAddr = Stack.MostRecentPropertyAddress;
		FDelegateProperty* DelegateProperty = CastField<FDelegateProperty>(Stack.MostRecentProperty);

		P_FINISH;
		P_NATIVE_BEGIN;
		GenericArray_Sort(ArrayAddr, ArrayProperty, DelegateAddr, DelegateProperty);
		P_NATIVE_END;
	}*/
};
