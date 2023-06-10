/*
  ==============================================================================

    AudioPlayer.h
    Created: 25 May 2023 12:56:15am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class AudioPlayer
{
public:
    // Constructors
    AudioPlayer();
    ~AudioPlayer();
    
    // Methods
    AudioFormatManager& getFormatManager();
    void startOrStop();
    void setTransportSourcePosition(double inPosition);
    void start();
    bool isPlaying();
    double getCurrentPosition();
    void stop();
    void setSource(PositionableAudioSource* const newSource,
                   int readAheadSize,
                   TimeSliceThread* readAheadThread,
                   double sourceSampleRateToCorrectFor);
    bool loadURLIntoTransport(const URL& inURL, TimeSliceThread& inThread);
    
private:
    // Fields
    std::unique_ptr<AudioDeviceManager> mAudioDeviceManager;
    AudioFormatManager mFormatManager;
    AudioSourcePlayer mAudioSourcePlayer;
    AudioTransportSource mTransportSource;
    std::unique_ptr<AudioFormatReaderSource> mCurrentAudioFileSource;
    
    // Methods
    
};
