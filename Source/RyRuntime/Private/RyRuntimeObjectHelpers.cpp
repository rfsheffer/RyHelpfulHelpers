// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#include "RyRuntimeObjectHelpers.h"
#include "UObject/ObjectRedirector.h"
#include "RyRuntimeModule.h"

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

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UObject* URyRuntimeObjectHelpers::LoadObjectFromPackage(UPackage* package, const FString& objectName)
{
    if(!package)
    {
        return nullptr;
    }

    return ::LoadObject<UObject>(package, *objectName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UObject* URyRuntimeObjectHelpers::LoadObject(const FString& fullObjectPath)
{
    UObject* LoadedObject = StaticLoadObject(UObject::StaticClass(), nullptr, *fullObjectPath, nullptr, LOAD_None, nullptr, true, nullptr);

#if WITH_EDITOR
    // Look at core redirects if we didn't find the object
    if(!LoadedObject)
    {
        FSoftObjectPath FixupObjectPath = fullObjectPath;
        if(FixupObjectPath.FixupCoreRedirects())
        {
            LoadedObject = ::LoadObject<UObject>(nullptr, *FixupObjectPath.ToString());
        }
    }
#endif

    while(UObjectRedirector* Redirector = Cast<UObjectRedirector>(LoadedObject))
    {
        LoadedObject = Redirector->DestinationObject;
    }

    return LoadedObject;
}
