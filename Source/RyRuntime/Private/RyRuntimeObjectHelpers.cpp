// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#include "RyRuntimeObjectHelpers.h"
#include "UObject/ObjectRedirector.h"
#include "RyRuntimeModule.h"
#include "UObject/Package.h"
#include "UObject/UObjectIterator.h"

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
void URyRuntimeObjectHelpers::GetObjectsInPackage(UPackage* package, TArray<UObject*>& ObjectsOut)
{
    if(!package)
    {
        return;
    }

    if(!package->IsFullyLoaded())
    {
        package->FullyLoad();
    }

    for(TObjectIterator<UObject> ObjIt; ObjIt; ++ObjIt)
    {
        UObject* Object = *ObjIt;
        if(Object->IsIn(package))
        {
            ObjectsOut.Add(Object);
        }
    }
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

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UClass* URyRuntimeObjectHelpers::GetParentClass(UClass* Class)
{
    if(!Class)
        return nullptr;

    return Class->GetSuperClass();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeObjectHelpers::GetClassHierarchy(UClass* Class, TArray<UClass*>& ClassHierarchy)
{
    UClass* NextClass = Class;
    while(NextClass && NextClass->GetSuperClass())
    {
        ClassHierarchy.Add(NextClass->GetSuperClass());
        NextClass = NextClass->GetSuperClass();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UObject* URyRuntimeObjectHelpers::GetClassDefaultObject(TSubclassOf<UObject> theClass)
{
    if(!theClass)
    {
        return nullptr;
    }

    return theClass->GetDefaultObject();
}
