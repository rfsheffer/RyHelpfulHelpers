// Copyright 2020-2022 Solar Storm Interactive

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
