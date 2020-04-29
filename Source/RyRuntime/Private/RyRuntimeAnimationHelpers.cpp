// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#include "RyRuntimeAnimationHelpers.h"
#include "RyRuntimeModule.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimMetaData.h"

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
    const TArray<UAnimMetaData*>& metaToCpy = MontageIn->GetMetaData();
    for(UAnimMetaData* metaData : metaToCpy)
    {
        if(metaData == nullptr)
            continue;

        NewMontage->AddMetaData(NewObject<UAnimMetaData>(NewMontage, metaData->GetClass(), 
                                                         MakeUniqueObjectName(NewMontage, metaData->GetClass(), metaData->GetFName()), 
                                                         RF_Public, metaData));
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
