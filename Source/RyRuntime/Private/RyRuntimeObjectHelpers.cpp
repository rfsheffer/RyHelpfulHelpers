// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#include "RyRuntimeObjectHelpers.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeObjectHelpers::IsLiveSoftObjectReference(const TSoftObjectPtr<UObject>& SoftObjectReference)
{
    if(SoftObjectReference.IsNull())
        return false;

    TPersistentObjectPtr<FSoftObjectPath> persistObjPtr(SoftObjectReference.ToSoftObjectPath());
    return persistObjPtr.Get(false) != nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UPackage* URyRuntimeObjectHelpers::FindOrLoadPackage(const FString& PackageName)
{
    UPackage* Pkg = FindPackage(nullptr, *PackageName);
    if(!Pkg)
    {
        Pkg = LoadPackage(nullptr, *PackageName, LOAD_NoWarn | LOAD_Quiet);
    }
    return Pkg;
}
