// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#include "RyEditorAnimationHelpers.h"
#include "RyEditorModule.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"
#include "Animation/Skeleton.h"

#include "Misc/MessageDialog.h"

#include "AssetRegistryModule.h"

#define LOCTEXT_NAMESPACE "RyEditor"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UAnimMontage* URyEditorAnimationHelpers::CreateMontageOfAnimations(const TArray<UAnimSequence*>& AnimsIn, const FName MontageName)
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

    FString assetName = MontageName.ToString() + TEXT("_Montage");
    FString assetPackagePath = FString::Printf(TEXT("%s/%s"), *basePackagePath, *assetName);

    UPackage* assetPackage = FindPackage(nullptr, *assetPackagePath);
    UAnimMontage* montage = nullptr;
    if(assetPackage)
    {
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Montage_PackageExists", "A package already exists with your montage name. Choose a different name."));
        return nullptr;
    }
    else
    {
        assetPackage = CreatePackage(NULL, *assetPackagePath);
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

        FAnimSegment newSegment;
        newSegment.AnimReference = sequence;
        newSegment.StartPos = curTime;
        newSegment.AnimStartTime = 0.0f;
        newSegment.AnimEndTime = sequence->SequenceLength;
        newSegment.AnimPlayRate = 1.0f;
        newSegment.LoopingCount = 1;
        montage->AddAnimCompositeSection(sequence->GetFName(), curTime);
        curTime += sequence->SequenceLength;

        montage->SlotAnimTracks[0].AnimTrack.AnimSegments.Add(newSegment);
    }

    montage->SequenceLength = curTime;

    montage->SetSkeleton(skel);

    // Make package dirty.
    montage->MarkPackageDirty();
    montage->PostEditChange();

    // Notify asset registry of new asset
    FAssetRegistryModule::AssetCreated(montage);

    return montage;
}

#undef LOCTEXT_NAMESPACE
