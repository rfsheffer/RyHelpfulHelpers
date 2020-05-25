// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#include "RyEditorSoundWaveHelpers.h"
#include "Sound/SoundWave.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyEditorSoundWaveHelpers::GetSpokenText(USoundWave* soundWave)
{
    if(!soundWave)
        return TEXT("");

    return soundWave->SpokenText;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyEditorSoundWaveHelpers::SetSubtitles(USoundWave* soundWave, const TArray<FRySubtitleCue>& subtitles)
{
    if(!soundWave)
        return;

    soundWave->Subtitles.Empty();
    for(const FRySubtitleCue& cue : subtitles)
    {
        FSubtitleCue cueIn;
        cueIn.Text = cue.Text;
        cueIn.Time = cue.Time;
        soundWave->Subtitles.Add(cueIn);
    }

    if(soundWave->CanModify())
    {
        soundWave->Modify();
    }
}
