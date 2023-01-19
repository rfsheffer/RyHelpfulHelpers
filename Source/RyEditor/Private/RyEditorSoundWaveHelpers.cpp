// Copyright 2020-2023 Solar Storm Interactive

#include "RyEditorSoundWaveHelpers.h"
#include "Sound/SoundWave.h"
#include "Runtime/Launch/Resources/Version.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyEditorSoundWaveHelpers::GetSpokenText(USoundWave* soundWave)
{
#if ENGINE_MAJOR_VERSION < 5
    if(!soundWave)
        return TEXT("");

    return soundWave->SpokenText;
#else
    return TEXT("");
#endif
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
