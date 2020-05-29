// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyEditorSoundWaveHelpers.generated.h"

USTRUCT(BlueprintType)
struct FRySubtitleCue
{
    GENERATED_BODY()

    /** The text to appear in the subtitle. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubtitleCue)
    FText Text;

    /** The time at which the subtitle is to be displayed, in seconds relative to the beginning of the line. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubtitleCue)
    float Time;

    FRySubtitleCue()
        : Time(0)
    {
    }
};

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to SoundWave assets
*/
UCLASS()
class RYEDITOR_API URyEditorSoundWaveHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    /** A localized version of the text that is actually spoken phonetically in the audio. */
    UFUNCTION(BlueprintPure, CallInEditor, Category = "RyEditor|SoundWaveHelpers")
    static FString GetSpokenText(class USoundWave* soundWave);

    /** Override the current subtitles in a SoundWave asset */
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|SoundWaveHelpers")
    static void SetSubtitles(class USoundWave* soundWave, const TArray<FRySubtitleCue>& subtitles);
};
