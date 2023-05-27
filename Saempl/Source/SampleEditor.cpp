/*
  ==============================================================================

    SampleEditor.cpp
    Created: 21 May 2023 3:00:50pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleEditor.h"

SampleEditor::SampleEditor()
{
    mAudioPlayer = std::make_unique<AudioPlayer>();
}

SampleEditor::~SampleEditor()
{
    
}
