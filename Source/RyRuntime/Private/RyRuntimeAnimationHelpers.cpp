// Copyright 2020-2023 Solar Storm Interactive

#include "RyRuntimeAnimationHelpers.h"
#include "RyRuntimeModule.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimMetaData.h"
#include "Runtime/Launch/Resources/Version.h"

#define LOCTEXT_NAMESPACE "RyRuntime"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UAnimMontage* URyRuntimeAnimationHelpers::CreateDynamicMontageFromMontage(UAnimMontage* MontageIn, const FName SlotOverride, 
                                                                          const float OverrideBlendIn, const float OverrideBlendOut, const float OverrideBlendOutTriggerTime)
{
    if(!MontageIn)
    {
        UE_LOG(LogRyRuntime, Error, TEXT("CreateDynamicMontageFromMontage : Invalid montage to copy?!"));
        return nullptr;
    }

    USkeleton* AssetSkeleton = MontageIn->GetSkeleton();
    if(!AssetSkeleton)
    {
        UE_LOG(LogRyRuntime, Error, TEXT("CreateDynamicMontageFromMontage : Montage to copy has no skeleton..."));
        return nullptr;
    }

    // Create a unique name based on the montages name. This makes things easier to debug.
    const FName duplicateName = MakeUniqueObjectName(GetTransientPackage(), UAnimMontage::StaticClass(), MontageIn->GetFName());

    // Create new montage
    UAnimMontage* NewMontage = NewObject<UAnimMontage>(GetTransientPackage(), duplicateName, RF_Transient);
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

    // Copy slot tracks and sections
    NewMontage->SlotAnimTracks = MontageIn->SlotAnimTracks;
    NewMontage->CompositeSections = MontageIn->CompositeSections;

    // Copy anim notifies
#if WITH_EDITORONLY_DATA
    NewMontage->AnimNotifyTracks = MontageIn->AnimNotifyTracks;
#endif
    NewMontage->Notifies = MontageIn->Notifies;

    // Other important stuff
PRAGMA_DISABLE_DEPRECATION_WARNINGS
    NewMontage->SequenceLength = MontageIn->SequenceLength;
PRAGMA_ENABLE_DEPRECATION_WARNINGS
    NewMontage->RateScale = MontageIn->RateScale;

    if(OverrideBlendIn != -1.0f)
    {
        NewMontage->BlendIn.SetBlendTime(OverrideBlendIn);
    }
    else
    {
        NewMontage->BlendIn.SetBlendTime(MontageIn->BlendIn.GetBlendTime());
    }
    if(OverrideBlendOut != -1.0f)
    {
        NewMontage->BlendOut.SetBlendTime(OverrideBlendOut);
    }
    else
    {
        NewMontage->BlendOut.SetBlendTime(MontageIn->BlendOut.GetBlendTime());
    }
    if(OverrideBlendOutTriggerTime != -1.0f)
    {
        NewMontage->BlendOutTriggerTime = OverrideBlendOutTriggerTime;
    }
    else
    {
        NewMontage->BlendOutTriggerTime = MontageIn->BlendOutTriggerTime;
    }

    if(!SlotOverride.IsNone())
    {
        for(FSlotAnimationTrack &slotTrack : NewMontage->SlotAnimTracks)
        {
            slotTrack.SlotName = SlotOverride;
        }
    }

    return NewMontage;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
int32 AddAnimCompositeSection(UAnimMontage* NewMontage, FName InSectionName, float StartTime)
{
    FCompositeSection NewSection;

    // make sure same name doesn't exists
    if ( InSectionName != NAME_None )
    {
        NewSection.SectionName = InSectionName;
    }
    else
    {
        // just give default name
        NewSection.SectionName = FName(*FString::Printf(TEXT("Section%d"), NewMontage->CompositeSections.Num()+1));
    }

    // we already have that name
    if ( NewMontage->GetSectionIndex(InSectionName)!=INDEX_NONE )
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("AnimCompositeSection : %s(%s) already exists. Choose different name."), 
            *NewSection.SectionName.ToString(), *InSectionName.ToString());
        return INDEX_NONE;
    }

#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1) || ENGINE_MAJOR_VERSION > 5
    NewSection.Link(NewMontage, StartTime);
#else
    NewSection.LinkMontage(NewMontage, StartTime);
#endif

    // we'd like to sort them in the order of time
    int32 NewSectionIndex = NewMontage->CompositeSections.Add(NewSection);

    // when first added, just make sure to link previous one to add me as next if previous one doesn't have any link
    // it's confusing first time when you add this data
    int32 PrevSectionIndex = NewSectionIndex-1;
    if ( NewMontage->CompositeSections.IsValidIndex(PrevSectionIndex) )
    {
        if (NewMontage->CompositeSections[PrevSectionIndex].NextSectionName == NAME_None)
        {
            NewMontage->CompositeSections[PrevSectionIndex].NextSectionName = InSectionName;
        }
    }

    return NewSectionIndex;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UAnimMontage* URyRuntimeAnimationHelpers::CreateDynamicMontageOfSequences(const TArray<UAnimSequence*>& SequencesIn, 
                                                                          const TArray<FName>& PerSequenceSectionNames,
                                                                          const TArray<int32>& LoopTimes,
                                                                          const FName AnimSlot /*= NAME_None*/,
                                                                          const float BlendIn /*= -1.0f*/,
                                                                          const float BlendOut /*= -1.0f*/,
                                                                          const float BlendOutTriggerTime /*= -1.0f*/,
                                                                          const bool EnableAutoBlendOut /*= true*/)
{
    if(!SequencesIn.Num())
    {
        UE_LOG(LogRyRuntime, Error, TEXT("CreateDynamicMontageFromSequences : With empty sequences array!"));
        return nullptr;
    }

    USkeleton* AssetSkeleton = SequencesIn[0]->GetSkeleton();
    if(!AssetSkeleton)
    {
        UE_LOG(LogRyRuntime, Error, TEXT("CreateDynamicMontageFromSequences : Sequences array contains null sequence!"));
        return nullptr;
    }

    // Create new montage
    UAnimMontage* NewMontage = NewObject<UAnimMontage>(GetTransientPackage(), NAME_None, RF_Transient);
    NewMontage->SetSkeleton(AssetSkeleton);

    FSlotAnimationTrack& animTrack = NewMontage->SlotAnimTracks[0];
    if(!AnimSlot.IsNone())
    {
        animTrack.SlotName = AnimSlot;
    }

    float curTime = 0.0f;
    TSet<FName> usedSections;
    for(int32 sequenceIndex = 0; sequenceIndex < SequencesIn.Num(); ++sequenceIndex)
    {
        UAnimSequence* sequence = SequencesIn[sequenceIndex];
        if(!sequence)
        {
            UE_LOG(LogRyRuntime, Warning, TEXT("CreateDynamicMontageFromSequences : Sequences array contains null sequence!"));
            continue;
        }
        if(!AssetSkeleton->IsCompatible(sequence->GetSkeleton()))
        {
            UE_LOG(LogRyRuntime, Warning, TEXT("CreateDynamicMontageFromSequences : Sequences array contains sequences which"
                                             " are not compatible with eachother (skeleton not compatible)!"));
            continue;
        }

        // Create the segment
        FAnimSegment& animSegment = animTrack.AnimTrack.AnimSegments.AddDefaulted_GetRef();
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1) || ENGINE_MAJOR_VERSION > 5
        animSegment.SetAnimReference(sequence);
#else
        animSegment.AnimReference = sequence;
#endif
        animSegment.StartPos = curTime;
        animSegment.AnimStartTime = 0.0f;
#if ENGINE_MAJOR_VERSION >= 5
        animSegment.AnimEndTime = sequence->GetPlayLength();
#else
        animSegment.AnimEndTime = sequence->SequenceLength;
#endif
        animSegment.AnimPlayRate = 1.0f;
        if(LoopTimes.IsValidIndex(sequenceIndex))
        {
            animSegment.LoopingCount = FMath::Max(1, LoopTimes[sequenceIndex]);
        }
        else
        {
            animSegment.LoopingCount = 1;
        }

        if(PerSequenceSectionNames.Num() > sequenceIndex && !usedSections.Contains(PerSequenceSectionNames[sequenceIndex]))
        {
            usedSections.Add(PerSequenceSectionNames[sequenceIndex]);

            // Add the section
            int32 sectionIndex = AddAnimCompositeSection(NewMontage, PerSequenceSectionNames[sequenceIndex], curTime);
            NewMontage->CompositeSections[sectionIndex].ChangeLinkMethod(EAnimLinkMethod::Relative);
        }

#if ENGINE_MAJOR_VERSION >= 5
        curTime += sequence->GetPlayLength() * animSegment.LoopingCount;
#else
        curTime += sequence->SequenceLength * animSegment.LoopingCount;
#endif
    }

    NewMontage->BlendIn.SetBlendTime(BlendIn);
    NewMontage->BlendOut.SetBlendTime(BlendOut);
    NewMontage->BlendOutTriggerTime = BlendOutTriggerTime;
    NewMontage->bEnableAutoBlendOut = EnableAutoBlendOut;

PRAGMA_DISABLE_DEPRECATION_WARNINGS
    NewMontage->SequenceLength = curTime;
PRAGMA_ENABLE_DEPRECATION_WARNINGS
    NewMontage->RateScale = 1.0f;

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

    const int32 SectionID = MontageIn->GetSectionIndexFromPosition(Position);
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

    const int32 SectionID = MontageIn->GetSectionIndex(SectionName);
    if(SectionID == INDEX_NONE)
        return;

    MontageIn->GetSectionStartAndEndTime(SectionID, OutStartTime, OutEndTime);
}

float URyRuntimeAnimationHelpers::GetMontageSectionTimeLeftFromPos(class UAnimMontage* MontageIn, const FName SectionName, const float Position)
{
    if(!MontageIn)
        return 0.0f;

    const int32 SectionID = MontageIn->GetSectionIndex(SectionName);
    if(SectionID == INDEX_NONE)
        return 0.0f;

    if(MontageIn->IsValidSectionIndex(SectionID + 1))
    {
        return FMath::Max(0.0f, MontageIn->GetAnimCompositeSection(SectionID + 1).GetTime() - Position);
    }
#if ENGINE_MAJOR_VERSION >= 5
    return FMath::Max(0.0f, MontageIn->GetPlayLength() - Position);
#else
    return FMath::Max(0.0f, MontageIn->SequenceLength - Position);
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UAnimMetaData* URyRuntimeAnimationHelpers::CreateAnimMetaDataInstance(TSubclassOf<UAnimMetaData> MetaDataClass)
{
    if(!MetaDataClass)
    {
        return nullptr;
    }
    
    UAnimMetaData* MetaDataInstance = NewObject<UAnimMetaData>(GetTransientPackage(), MetaDataClass, NAME_None, RF_Transient);
    if (!MetaDataInstance)
    {
        UE_LOG(LogRyRuntime, Warning, TEXT("Failed to create anim metadata instance for %s"), *MetaDataClass->GetName());
    }

    return MetaDataInstance;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeAnimationHelpers::GetAllMontages(UAnimInstance* animInstance, TArray<UAnimMontage*>& montagesOut)
{
    if(animInstance)
    {
        for(FAnimMontageInstance*& instance : animInstance->MontageInstances)
        {
            if(instance)
            {
                montagesOut.Add(instance->Montage);
            }
        }
    }
}

#undef LOCTEXT_NAMESPACE
