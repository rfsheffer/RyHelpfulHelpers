// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyEditorSkeletalMeshHelpers.h"
#include "Engine/SkeletalMesh.h"

//----------------------------------------------------------------------------------------------------------------------
/**
*/
void URyEditorSkeletalMeshHelpers::InvalidateDeriveDataCacheGUID(USkeletalMesh* skeletalMesh)
{
    if(skeletalMesh)
    {
        skeletalMesh->PreEditChange(nullptr);
        skeletalMesh->InvalidateDeriveDataCacheGUID();
        skeletalMesh->PostEditChange();
    }
}
