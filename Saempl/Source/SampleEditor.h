/*
  ==============================================================================

    SampleEditor.h
    Created: 21 May 2023 3:00:50pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "AudioPlayer.h"

class SampleEditor
{
public:
    SampleEditor();
    ~SampleEditor();
    
private:
    std::unique_ptr<AudioPlayer> mAudioPlayer;
    
};
