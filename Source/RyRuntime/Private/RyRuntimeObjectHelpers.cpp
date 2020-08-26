// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

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
UPackage* URyRuntimeObjectHelpers::GetPackageOfObject(UObject* object)
{
    if(!object)
        return nullptr;

    return object->GetOutermost();
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
void URyRuntimeObjectHelpers::GetClassHierarchy(UClass* Class, TArray<UClass*>& ClassHierarchy, const bool includeSelf)
{
    UClass* NextClass = Class;
    if(NextClass && includeSelf)
    {
        ClassHierarchy.Add(NextClass);
    }
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
#if RY_INCLUDE_DANGEROUS_FUNCTIONS
    if(!theClass)
    {
        return nullptr;
    }

    return theClass->GetDefaultObject();
#else
    return nullptr;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeObjectHelpers::SetObjectPropertyValue(UObject* object, const FName PropertyName, const FString& Value, const bool PrintWarnings)
{
#if RY_INCLUDE_DANGEROUS_FUNCTIONS
    if(!object)
    {
        return false;
    }

#if ENGINE_MINOR_VERSION < 25
    UProperty *FoundProperty = object->GetClass()->FindPropertyByName(PropertyName);
#else
    FProperty *FoundProperty = object->GetClass()->FindPropertyByName(PropertyName);
#endif
    if(FoundProperty)
    {
        void *PropertyPtr = FoundProperty->ContainerPtrToValuePtr<void>(object);
        check(PropertyPtr);
#if ENGINE_MINOR_VERSION < 25
        if(UNumericProperty *pIntProp = Cast<UNumericProperty>(FoundProperty))
#else
        if(FNumericProperty *pIntProp = CastField<FNumericProperty>(FoundProperty))
#endif
        {
            if(Value.IsNumeric())
            {
                pIntProp->SetNumericPropertyValueFromString(PropertyPtr, *Value);
                return true;
            }
            else
            {
                if(PrintWarnings)
                {
                    UE_LOG(LogRyRuntime, Warning, TEXT("SetObjectPropertyValue: Property named '%s' is numeric but the Value string is not"), *PropertyName.ToString());
                }
                return false;
            }
        }
#if ENGINE_MINOR_VERSION < 25
        else if(UBoolProperty *pBoolProp = Cast<UBoolProperty>(FoundProperty))
#else
        else if(FBoolProperty *pBoolProp = CastField<FBoolProperty>(FoundProperty))
#endif
        {
            pBoolProp->SetPropertyValue(PropertyPtr, FCString::ToBool(*Value));
            return true;
        }
#if ENGINE_MINOR_VERSION < 25
        else if(UStructProperty* StructProperty = Cast<UStructProperty>(FoundProperty))
#else
        else if(FStructProperty* StructProperty = CastField<FStructProperty>(FoundProperty))
#endif
        {
            FName StructType = StructProperty->Struct->GetFName();
            if(StructType == NAME_LinearColor)
            {
                FLinearColor *pCol = (FLinearColor*)PropertyPtr;
                return pCol->InitFromString(Value);
            }
            else if(StructType == NAME_Color)
            {
                FColor *pCol = (FColor*)PropertyPtr;
                return pCol->InitFromString(Value);
            }
            else if(StructType == NAME_Vector)
            {
                FVector *pVec = (FVector*)PropertyPtr;
                return pVec->InitFromString(Value);
            }
            else if(StructType == NAME_Rotator)
            {
                FRotator *pRot = (FRotator*)PropertyPtr;
                return pRot->InitFromString(Value);
            }
            else if(StructType == NAME_Transform)
            {
                FTransform *pTrans = (FTransform*)PropertyPtr;
                return pTrans->InitFromString(Value);
            }
        }

        if(PrintWarnings)
        {
            UE_LOG(LogRyRuntime, Warning, TEXT("SetObjectPropertyValue: Unsupported property named '%s'"), *PropertyName.ToString());
        }
    }
    else if(PrintWarnings)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("SetObjectPropertyValue: Unable to find property in object named '%s'"), *PropertyName.ToString());
    }
#endif // RY_INCLUDE_DANGEROUS_FUNCTIONS

    return false;
}
