// Copyright 2020-2023 Solar Storm Interactive

#include "RyEditorAnimationHelpers.h"
#include "RyRuntimeObjectHelpers.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"
#include "Animation/Skeleton.h"

#include "Misc/MessageDialog.h"
#include "Runtime/Launch/Resources/Version.h"

#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1) || ENGINE_MAJOR_VERSION > 5
#include "AssetRegistry/AssetRegistryModule.h"
#else
#include "AssetRegistryModule.h"
#endif

#define LOCTEXT_NAMESPACE "RyEditor"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UAnimMontage* URyEditorAnimationHelpers::CreateMontageOfSequences(const TArray<UAnimSequence*>& AnimsIn, const FName MontageName)
{
    if(MontageName.IsNone())
        return nullptr;

    USkeleton *skel = nullptr;
    FString basePackagePath;
    for(const UAnimSequence* section : AnimsIn)
    {
        if(section)
        {
            if(basePackagePath.IsEmpty())
            {
                section->GetPathName(nullptr, basePackagePath);
                basePackagePath = FPaths::GetPath(basePackagePath);
            }
            if(!skel)
            {
                skel = section->GetSkeleton();
            }
            else if(skel != section->GetSkeleton())
            {
                FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Montage_IncompatSkel", "Cannot create montage of sequences with different skeletons!"));
                return nullptr;
            }
        }
    }

    if(!skel || basePackagePath.IsEmpty())
        return nullptr;

    const FString assetName = MontageName.ToString() + TEXT("_Montage");
    const FString assetPackagePath = FString::Printf(TEXT("%s/%s"), *basePackagePath, *assetName);

    UPackage* assetPackage = URyRuntimeObjectHelpers::FindOrLoadPackage(*assetPackagePath);
    UAnimMontage* montage = nullptr;
    if(assetPackage)
    {
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Montage_PackageExists", "A package already exists with your montage name. Choose a different name."));
        return nullptr;
    }
    else
    {
#if ENGINE_MAJOR_VERSION == 5 || ENGINE_MINOR_VERSION >= 26
        assetPackage = CreatePackage(*assetPackagePath);
#else
        assetPackage = CreatePackage(nullptr, *assetPackagePath);
#endif
        if(!assetPackage)
        {
            FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Montage_NoCreatePackage", "Unable to create package for Montage asset!"));
            return nullptr;
        }

        montage = NewObject<UAnimMontage>(assetPackage, *assetName, RF_Public | RF_Standalone);
        if(!montage)
        {
            FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Montage_NoCreateAsset", "Unable to create Montage asset!"));
            return nullptr;
        }
    }

    // Create track of sequences
    float curTime = 0.0f;
    if(!montage->SlotAnimTracks.Num())
    {
        montage->SlotAnimTracks.AddDefaulted();
    }

    montage->CompositeSections.Empty();
    for(int32 createSeqIndex = 0; createSeqIndex < AnimsIn.Num(); ++createSeqIndex)
    {
        UAnimSequence* sequence = AnimsIn[createSeqIndex];
        if(!sequence)
            continue;

        // Create the segment
        FAnimSegment newSegment;
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1) || ENGINE_MAJOR_VERSION > 5
        newSegment.SetAnimReference(sequence);
#else
        newSegment.AnimReference = sequence;
#endif
        newSegment.StartPos = curTime;
        newSegment.AnimStartTime = 0.0f;
#if ENGINE_MAJOR_VERSION >= 5
        newSegment.AnimEndTime = sequence->GetPlayLength();
#else
        newSegment.AnimEndTime = sequence->SequenceLength;
#endif
        newSegment.AnimPlayRate = 1.0f;
        newSegment.LoopingCount = 1;
        montage->SlotAnimTracks[0].AnimTrack.AnimSegments.Add(newSegment);

        // Add the section
        const int32 sectionIndex = montage->AddAnimCompositeSection(sequence->GetFName(), curTime);
        montage->CompositeSections[sectionIndex].ChangeLinkMethod(EAnimLinkMethod::Relative);

#if ENGINE_MAJOR_VERSION >= 5
        curTime += sequence->GetPlayLength();
#else
        curTime += sequence->SequenceLength;
#endif
    }

#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 2) || ENGINE_MAJOR_VERSION > 5
    montage->SetCompositeLength(curTime);
#else
PRAGMA_DISABLE_DEPRECATION_WARNINGS
    montage->SequenceLength = curTime;
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#endif

    montage->SetSkeleton(skel);

    // Make package dirty.
    montage->MarkPackageDirty();
    montage->PostEditChange();

    // Notify asset registry of new asset
    FAssetRegistryModule::AssetCreated(montage);

    return montage;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyEditorAnimationHelpers::CreateMontageSectionsFromSegments(UAnimMontage* MontageIn)
{
    if(!MontageIn || MontageIn->SlotAnimTracks.Num() == 0)
        return;

    MontageIn->CompositeSections.Empty();
    float curTime = 0;
    for(int32 segIndex = 0; segIndex < MontageIn->SlotAnimTracks[0].AnimTrack.AnimSegments.Num(); ++segIndex)
    {
        FAnimSegment& segment = MontageIn->SlotAnimTracks[0].AnimTrack.AnimSegments[segIndex];
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1) || ENGINE_MAJOR_VERSION > 5
        if(segment.GetAnimReference())
#else
    	if(segment.AnimReference)
#endif
        {
    	    
            const int32 sectionIndex = MontageIn->AddAnimCompositeSection(
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1) || ENGINE_MAJOR_VERSION > 5
				segment.GetAnimReference()->GetFName(),
#else
				segment.AnimReference->GetFName(),
#endif
            	curTime);
            MontageIn->CompositeSections[sectionIndex].ChangeLinkMethod(EAnimLinkMethod::Relative);
#if ENGINE_MAJOR_VERSION >= 5
#if ENGINE_MINOR_VERSION >= 1 || ENGINE_MAJOR_VERSION > 5
            curTime += segment.GetAnimReference()->GetPlayLength();
#else
    		curTime += segment.AnimReference->GetPlayLength();
#endif
#else
            curTime += segment.AnimReference->SequenceLength;
#endif
        }
    }

    MontageIn->MarkPackageDirty();
    MontageIn->PostEditChange();
}

#undef LOCTEXT_NAMESPACE
