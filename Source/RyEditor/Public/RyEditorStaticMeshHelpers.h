// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#pragma once

#include "RyEditorStaticMeshHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to static mesh assets
*/
UCLASS()
class RYEDITOR_API URyEditorStaticMeshHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    // Apply Convex Decomposition to a mesh. This overrides the meshes simple collision.
    // This is useful for cases where you want to apply auto generated collision to a number of meshes.
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|AnimationHelpers")
    static void ApplyConvexDecomposition(class UStaticMesh* StaticMesh, int32 InHullCount = 4, int32 InMaxHullVerts = 16, int32 InHullPrecision = 100000);
};
