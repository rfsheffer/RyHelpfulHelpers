// Copyright 2020-2022 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyEditorStaticMeshHelpers.h"

#include "Editor.h"
#include "Engine/StaticMesh.h"
//#include "StaticMeshResources.h"
//#include "BusyCursor.h"
#include "Editor/UnrealEd/Private/GeomFitUtils.h"
//#include "EditorViewportCommands.h"
//#include "Editor/UnrealEd/Private/ConvexDecompTool.h"
#include "PhysicsEngine/BodySetup.h"
#include "Subsystems/AssetEditorSubsystem.h"

// Set USE_ASYNC_DECOMP to zero to go back to the fully synchronous; blocking version of V-HACD
// #ifndef USE_ASYNC_DECOMP
// #define USE_ASYNC_DECOMP 1
// #endif

void URyEditorStaticMeshHelpers::SetCollisionTraceFlag(class UStaticMesh* StaticMesh, TEnumAsByte<enum ECollisionTraceFlag> collisionTraceFlag)
{
#if ENGINE_MAJOR_VERSION == 5 || ENGINE_MINOR_VERSION >= 27
    if(StaticMesh && StaticMesh->GetBodySetup())
#else
    if(StaticMesh && StaticMesh->BodySetup)
#endif
    {
        UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
        bool bStaticMeshIsEdited = false;
        if(AssetEditorSubsystem->FindEditorForAsset(StaticMesh, false))
        {
            AssetEditorSubsystem->CloseAllEditorsForAsset(StaticMesh);
            bStaticMeshIsEdited = true;
        }
        
#if ENGINE_MAJOR_VERSION == 5 || ENGINE_MINOR_VERSION >= 27
        StaticMesh->GetBodySetup()->CollisionTraceFlag = collisionTraceFlag;
        StaticMesh->GetBodySetup()->Modify();
#else
        StaticMesh->BodySetup->CollisionTraceFlag = collisionTraceFlag;
        StaticMesh->BodySetup->Modify();
#endif

        // refresh collision change back to static mesh components
        RefreshCollisionChange(*StaticMesh);

        // Request re-building of mesh with new collision shapes
        StaticMesh->PostEditChange();

        // Reopen MeshEditor on this mesh if the MeshEditor was previously opened in it
        if(bStaticMeshIsEdited)
        {
            AssetEditorSubsystem->OpenEditorForAsset(StaticMesh);
        }
    }
}

void URyEditorStaticMeshHelpers::SetCanAffectNavMesh(class UStaticMeshComponent* StaticMesh, bool canAffect)
{
    if(StaticMesh)
    {
        StaticMesh->SetCanEverAffectNavigation(canAffect);
    }
}
