// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#include "RyRuntimeAnimationHelpers.h"
#include "RyRuntimeModule.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"
#include "Animation/Skeleton.h"

#define LOCTEXT_NAMESPACE "RyRuntime"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UAnimMontage* URyRuntimeAnimationHelpers::CreateDynamicMontageFromMontage(UAnimMontage* MontageIn)
{
    if(!MontageIn)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("CreateDynamicMontageFromMontage : Invalid montage to copy?!"));
        return nullptr;
    }

    USkeleton* AssetSkeleton = MontageIn->GetSkeleton();
    if(!AssetSkeleton)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("CreateDynamicMontageFromMontage : Montage to copy has no skeleton..."));
        return nullptr;
    }

    // Create new montage
    UAnimMontage* NewMontage = NewObject<UAnimMontage>();
    NewMontage->SetSkeleton(AssetSkeleton);

    // Copy tracks and sections
    const TArray<class UAnimMetaData*>& metaToCpy = MontageIn->GetMetaData();
    for(UAnimMetaData* metaData : metaToCpy)
    {
        NewMontage->AddMetaData(metaData);
    }
    NewMontage->SlotAnimTracks = MontageIn->SlotAnimTracks;
    NewMontage->CompositeSections = MontageIn->CompositeSections;

    NewMontage->SequenceLength = MontageIn->SequenceLength;
    NewMontage->RateScale = MontageIn->RateScale;

    NewMontage->BlendIn.SetBlendTime(MontageIn->BlendIn.GetBlendTime());
    NewMontage->BlendOut.SetBlendTime(MontageIn->BlendOut.GetBlendTime());
    NewMontage->BlendOutTriggerTime = MontageIn->BlendOutTriggerTime;

    return NewMontage;
}

#undef LOCTEXT_NAMESPACE
