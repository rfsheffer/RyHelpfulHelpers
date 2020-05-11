// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#include "RyEditorStaticMeshHelpers.h"

#include "Engine/StaticMesh.h"
#include "StaticMeshResources.h"
#include "BusyCursor.h"
#include "Editor/UnrealEd/Private/GeomFitUtils.h"
#include "EditorViewportCommands.h"
#include "Editor/UnrealEd/Private/ConvexDecompTool.h"
#include "PhysicsEngine/BodySetup.h"

// Set USE_ASYNC_DECOMP to zero to go back to the fully synchronous; blocking version of V-HACD
// #ifndef USE_ASYNC_DECOMP
// #define USE_ASYNC_DECOMP 1
// #endif

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyEditorStaticMeshHelpers::ApplyConvexDecomposition(UStaticMesh* StaticMesh, int32 InHullCount /*= 4*/, int32 InMaxHullVerts /*= 16*/, int32 InHullPrecision /*= 100000*/)
{
    // Check we have a selected StaticMesh
    if(StaticMesh && StaticMesh->RenderData && InHullCount > 0 && InMaxHullVerts > 0 && InHullPrecision > 0)
    {
        FStaticMeshLODResources& LODModel = StaticMesh->RenderData->LODResources[0];

        // Start a busy cursor so the user has feedback while waiting
        const FScopedBusyCursor BusyCursor;

        // Make vertex buffer
        int32 NumVerts = LODModel.VertexBuffers.StaticMeshVertexBuffer.GetNumVertices();
        TArray<FVector> Verts;
        for(int32 i = 0; i < NumVerts; i++)
        {
            FVector Vert = LODModel.VertexBuffers.PositionVertexBuffer.VertexPosition(i);
            Verts.Add(Vert);
        }

        // Grab all indices
        TArray<uint32> AllIndices;
        LODModel.IndexBuffer.GetCopy(AllIndices);

        // Only copy indices that have collision enabled
        TArray<uint32> CollidingIndices;
        for(const FStaticMeshSection& Section : LODModel.Sections)
        {
            if(Section.bEnableCollision)
            {
                for(uint32 IndexIdx = Section.FirstIndex; IndexIdx < Section.FirstIndex + (Section.NumTriangles * 3); IndexIdx++)
                {
                    CollidingIndices.Add(AllIndices[IndexIdx]);
                }
            }
        }

        //ClearSelectedPrims();

        // Make sure rendering is done - so we are not changing data being used by collision drawing.
        FlushRenderingCommands();

        // Get the BodySetup we are going to put the collision into
        UBodySetup* bs = StaticMesh->BodySetup;
        if(bs)
        {
            bs->RemoveSimpleCollision();
        }
        else
        {
            // Otherwise, create one here.
            StaticMesh->CreateBodySetup();
            bs = StaticMesh->BodySetup;
        }

        // Run actual util to do the work (if we have some valid input)
        if(Verts.Num() >= 3 && CollidingIndices.Num() >= 3)
        {
            // RY NOTE: I don't think this should be async unless I do some leg work to ensure the user cannot create some race condition.
// #if USE_ASYNC_DECOMP
//             // If there is currently a decomposition already in progress we release it.
//             if(DecomposeMeshToHullsAsync)
//             {
//                 DecomposeMeshToHullsAsync->Release();
//             }
//             // Begin the convex decomposition process asynchronously
//             DecomposeMeshToHullsAsync = CreateIDecomposeMeshToHullAsync();
//             DecomposeMeshToHullsAsync->DecomposeMeshToHullsAsyncBegin(bs, Verts, CollidingIndices, InHullCount, InMaxHullVerts, InHullPrecision);
// #else
//             DecomposeMeshToHulls(bs, Verts, CollidingIndices, InHullCount, InMaxHullVerts, InHullPrecision);
// #endif

            DecomposeMeshToHulls(bs, Verts, CollidingIndices, InHullCount, InMaxHullVerts, InHullPrecision);
        }

        // Enable collision, if not already
//         if(!Viewport->GetViewportClient().IsShowSimpleCollisionChecked())
//         {
//             Viewport->GetViewportClient().ToggleShowSimpleCollision();
//         }

        // refresh collision change back to staticmesh components
        RefreshCollisionChange(*StaticMesh);

        // Mark mesh as dirty
        StaticMesh->MarkPackageDirty();

        // Update screen.
        //Viewport->RefreshViewport();

        StaticMesh->bCustomizedCollision = true;	//mark the static mesh for collision customization
    }
}
