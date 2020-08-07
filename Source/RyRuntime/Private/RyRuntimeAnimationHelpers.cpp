// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

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

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeAnimationHelpers::GetMontageSectionNames(class UAnimMontage* MontageIn, TArray<FName>& NamesOut)
{
    if(!MontageIn)
        return;

    for(const FCompositeSection& section : MontageIn->CompositeSections)
    {
        NamesOut.Add(section.SectionName);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeAnimationHelpers::MontageHasSection(class UAnimMontage* MontageIn, const FName SectionName)
{
    if(!MontageIn)
        return false;

    return MontageIn->GetSectionIndex(SectionName) != INDEX_NONE;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FName URyRuntimeAnimationHelpers::GetMontageSectionNameFromPosition(class UAnimMontage* MontageIn, const float Position)
{
    if(!MontageIn)
        return NAME_None;

    int32 SectionID = MontageIn->GetSectionIndexFromPosition(Position);
    if(SectionID == INDEX_NONE)
        return NAME_None;

    return MontageIn->GetSectionName(SectionID);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeAnimationHelpers::GetMontageSectionStartAndEndTime(class UAnimMontage* MontageIn, const FName SectionName, float& OutStartTime, float& OutEndTime)
{
    if(!MontageIn)
        return;

    int32 SectionID = MontageIn->GetSectionIndex(SectionName);
    if(SectionID == INDEX_NONE)
        return;

    MontageIn->GetSectionStartAndEndTime(SectionID, OutStartTime, OutEndTime);
}

float URyRuntimeAnimationHelpers::GetMontageSectionTimeLeftFromPos(class UAnimMontage* MontageIn, const FName SectionName, const float Position)
{
    if(!MontageIn)
        return 0.0f;

    int32 SectionID = MontageIn->GetSectionIndex(SectionName);
    if(SectionID == INDEX_NONE)
        return 0.0f;

    if(MontageIn->IsValidSectionIndex(SectionID + 1))
    {
        return FMath::Max(0.0f, MontageIn->GetAnimCompositeSection(SectionID + 1).GetTime() - Position);
    }
    else
    {
        return FMath::Max(0.0f, MontageIn->SequenceLength - Position);
    }
}

#undef LOCTEXT_NAMESPACE
