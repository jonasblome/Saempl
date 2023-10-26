/*
  ==============================================================================

    SampleItemViewModel.h
    Created: 22 May 2023 6:31:17am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleItem.h"
#include "SampleEditor.h"

class SampleItemViewModel
{
public:
    // Constructors
    SampleItemViewModel();
    ~SampleItemViewModel();
    
    // Methods
    AudioFormatManager& getAudioFormatManager();
    void startOrStopAudio();
    void setAudioReadheadPosition(double inPosition);
    void startAudio();
    bool isPlayingAudio();
    double getCurrentPosition();
    void stopAudio();
    bool loadURLIntoTransport(URL const & inURL, TimeSliceThread& inThread);
    
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleItemViewModel)
    
    // Fields
    std::unique_ptr<SampleEditor> mSampleEditor;
    SampleItem mLoadedSample;
    
    // Methods
    
};
