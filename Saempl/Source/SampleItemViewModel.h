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
    AudioFormatManager& getFormatManager();
    void startOrStop();
    void setPosition(double position);
    void start();
    bool isPlaying();
    double getCurrentPosition();
    void stop();
    void setSource(PositionableAudioSource* const newSource,
                   int readAheadSize = 0,
                   TimeSliceThread* readAheadThread = nullptr,
                   double sourceSampleRateToCorrectFor = 0.0);
    bool loadURLIntoTransport(const URL& audioURL, TimeSliceThread& inThread);
    
private:
    std::unique_ptr<SampleEditor> mSampleEditor;
    SampleItem mLoadedSample;
};
