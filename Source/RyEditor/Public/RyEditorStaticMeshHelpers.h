// Copyright 2020-2022 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Launch/Resources/Version.h"
#if ENGINE_MAJOR_VERSION == 5 || ENGINE_MINOR_VERSION >= 27
#include "PhysicsCore/Public/BodySetupEnums.h"
#else
#include "PhysicsEngine/BodySetupEnums.h"
#endif

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

    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|StaticMeshHelpers")
    static void SetCollisionTraceFlag(class UStaticMesh* StaticMesh, TEnumAsByte<enum ECollisionTraceFlag> collisionTraceFlag);

    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|StaticMeshHelpers")
    static void SetCanAffectNavMesh(class UStaticMeshComponent* StaticMesh, bool canAffect);
};
