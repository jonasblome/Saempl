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
    // Constructors
    SampleEditor();
    ~SampleEditor();
    
    // Methods
    AudioFormatManager& getFormatManager();
    void startOrStop();
    void setPosition(double position);
    void start();
    bool isPlaying();
    double getCurrentPosition();
    void stop();
    void setSource(PositionableAudioSource* const newSource,
                   int readAheadSize,
                   TimeSliceThread* readAheadThread,
                   double sourceSampleRateToCorrectFor);
    bool loadURLIntoTransport(const URL& audioURL, TimeSliceThread& inThread);
    
private:
    // Fields
    std::unique_ptr<AudioPlayer> mAudioPlayer;
    
    // Methods
    
};
