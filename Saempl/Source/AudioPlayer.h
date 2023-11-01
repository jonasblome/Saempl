/*
  ==============================================================================

    AudioPlayer.h
    Created: 25 May 2023 12:56:15am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

/**
 Handles audio playback for different audio formats.
 */
class AudioPlayer
{
public:
    AudioPlayer();
    ~AudioPlayer();
    /**
     @returns the player's audio format manager.
     */
    AudioFormatManager& getFormatManager();
    /**
     Starts or stops the audio playback.
     */
    void startOrStop();
    /**
     Sets the readhead position of the audio player.
     
     @param position the position in seconds.
     */
    void setTransportSourcePosition(double inPosition);
    /**
     Starts audio playback.
     */
    void start();
    /**
     @returns if the player currently plays back audio.
     */
    bool isPlaying();
    /**
     @returns the current readhead position of the audio player.
     */
    double getCurrentPosition();
    /**
     Stops the audio playback.
     */
    void stop();
    /**
     Loads the given URL into the audio transport source.
     
     @param audioURL the URL to load into the transport source.
     @param inThread the thread to use for reading ahead in the audio file.
     
     @returns if the loading was successful.
     */
    bool loadURLIntoTransport(URL const & inURL, TimeSliceThread& inThread);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(AudioPlayer)
    
    std::unique_ptr<AudioDeviceManager> mAudioDeviceManager;
    AudioFormatManager mFormatManager;
    AudioSourcePlayer mAudioSourcePlayer;
    AudioTransportSource mTransportSource;
    std::unique_ptr<AudioFormatReaderSource> mCurrentAudioFileSource;
};
