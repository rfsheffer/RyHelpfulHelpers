// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#include "RyEditorBlueprintHelpers.h"
#include "RyEditorModule.h"

// Creating new blueprints
#include "Kismet2/KismetEditorUtilities.h"
#include "PackageTools.h"
#include "KismetCompilerModule.h"
#include "AssetRegistryModule.h"
#include "Kismet2/CompilerResultsLog.h"

// Helpers
#include "RyRuntimeObjectHelpers.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "RyEditor"

static FName RyUtilitiesContext = FName(TEXT("RyEditor Blueprint Helpers"));

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UBlueprint* URyEditorBlueprintHelpers::CreateBlueprint(const FString& packagePath, TSubclassOf<class UObject> parentClass, bool getExistingBlueprint)
{
    if(!parentClass)
    {
        UE_LOG(LogRyEditor, Error, TEXT("CreateBlueprint called with invalid parentClass!"));
        return nullptr;
    }
    if(!packagePath.StartsWith(TEXT("/")))
    {
        UE_LOG(LogRyEditor, Error, TEXT("CreateBlueprint called with what appears to be an invalid package path! Does not start with '/'"));
        return nullptr;
    }

    const FString packageName = UPackageTools::SanitizePackageName(packagePath);
    const FString assetName = FPaths::GetBaseFilename(packageName);

    // Make sure we can create the asset without conflicts
    UPackage *package = URyRuntimeObjectHelpers::FindOrLoadPackage(packageName);
    if(package != nullptr)
    {
        // Load up the current blueprint asset
        UBlueprint* pBlueprint = LoadObject<UBlueprint>(package, *assetName);
        if(!pBlueprint)
        {
            UE_LOG(LogRyEditor, Error, TEXT("CreateBlueprint called with a package path to a package which exists that does not contain a blueprint asset!"));
        }
        else if (getExistingBlueprint && pBlueprint->ParentClass != parentClass)
        {
            UE_LOG(LogRyEditor, Warning, TEXT("CreateBlueprint called with a package path to a blueprint asset but the"
                                                " blueprints parent class is not the parent class supplied to CreateBlueprint!"));
        }
        return getExistingBlueprint ? pBlueprint : nullptr;
    }

    if(!FKismetEditorUtilities::CanCreateBlueprintOfClass(parentClass))
    {
        UE_LOG(LogRyEditor, Error, TEXT("CreateBlueprint called with a parentClass which cannot have a blueprint created of it!"));
        return nullptr;
    }

    package = CreatePackage(nullptr, *packageName);
    UBlueprint* newBlueprint = nullptr;
    EObjectFlags flags = RF_Public | RF_Standalone | RF_Transactional;

    UClass* blueprintClass = nullptr;
    UClass* blueprintGeneratedClass = nullptr;

    IKismetCompilerInterface& KismetCompilerModule = FModuleManager::LoadModuleChecked<IKismetCompilerInterface>("KismetCompiler");
    KismetCompilerModule.GetBlueprintTypesForClass(parentClass, blueprintClass, blueprintGeneratedClass);

    newBlueprint = FKismetEditorUtilities::CreateBlueprint(parentClass, package, *assetName, BPTYPE_Normal, blueprintClass,
                                                           blueprintGeneratedClass, RyUtilitiesContext);

    if(newBlueprint)
    {
        // Notify the asset registry
        FAssetRegistryModule::AssetCreated(newBlueprint);

        // Mark the package dirty...
        package->MarkPackageDirty();
    }
    return newBlueprint;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UBlueprint* URyEditorBlueprintHelpers::GetBlueprint(const FString& packagePath)
{
    const FString packageName = UPackageTools::SanitizePackageName(packagePath);
    const FString assetName = FPaths::GetBaseFilename(packageName);

    // Make sure we can create the asset without conflicts
    UPackage *package = URyRuntimeObjectHelpers::FindOrLoadPackage(packageName);
    if(package != nullptr)
    {
        // Load up the current blueprint asset, if it is a blueprint...
        return LoadObject<UBlueprint>(package, *assetName);
    }

    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UBlueprintGeneratedClass* URyEditorBlueprintHelpers::GetBlueprintGeneratedClass(class UBlueprint* blueprint)
{
    if(!blueprint)
    {
        return nullptr;
    }

    return Cast<UBlueprintGeneratedClass>(blueprint->GeneratedClass);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UBlueprintGeneratedClass* URyEditorBlueprintHelpers::GetBlueprintGeneratedClassAtPath(const FString& packagePath)
{
    UPackage *package = URyRuntimeObjectHelpers::FindOrLoadPackage(packagePath);
    if(!package)
    {
        UE_LOG(LogRyEditor, Error, TEXT("GetBlueprintGeneratedClassAtPath called with invalid package path!"));
        return nullptr;
    }

    FString generatedClassName = FString::Printf(TEXT("%s_C"), *FPaths::GetBaseFilename(packagePath));
    return LoadObject<UBlueprintGeneratedClass>(package, *generatedClassName, NULL, LOAD_None, NULL);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyEditorBlueprintHelpers::CompileBlueprint(UBlueprint* blueprint, TArray<FString>& compilerMessagesOut)
{
    if(!blueprint)
    {
        return false;
    }

    FCompilerResultsLog log;
    FKismetEditorUtilities::CompileBlueprint(blueprint, EBlueprintCompileOptions::None, &log);
    for(const TSharedRef<FTokenizedMessage>& message : log.Messages)
    {
        compilerMessagesOut.Add(message->ToText().ToString());
    }
    return log.NumErrors == 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyEditorBlueprintHelpers::AddComponentsToBlueprint(UBlueprint* blueprint, TArray<UActorComponent*> Components, bool bHarvesting /*= false*/, bool bKeepMobility /*= false*/)
{
    if(!blueprint)
    {
        return;
    }

    FKismetEditorUtilities::AddComponentsToBlueprint(blueprint, Components, bHarvesting, nullptr, bKeepMobility);
}

#undef LOCTEXT_NAMESPACE
