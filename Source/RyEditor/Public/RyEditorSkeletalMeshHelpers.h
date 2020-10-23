// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyEditorSkeletalMeshHelpers.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
* Static Helper functions related to skeletal mesh assets
*/
UCLASS()
class RYEDITOR_API URyEditorSkeletalMeshHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    /**
    * Force the creation of a new GUID use to build the derive data cache key.
    * Next time a build happen the whole skeletal mesh will be rebuild.
    * Use this when you change stuff not in the skeletal mesh ddc key, like the geometry (import, re-import)
    * Every big data should not be in the ddc key and should use this function, because its slow to create a key with big data.
    */
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|SkeletalMeshHelpers")
    static void InvalidateDeriveDataCacheGUID(class USkeletalMesh* skeletalMesh);
};
