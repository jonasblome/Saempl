/*
 ==============================================================================
 
 AudioPlayer.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once
#include "JuceHeader.h"

/**
 Handles audio playback for different audio formats.
 */
class AudioPlayer
:
private Timer
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
     
     @returns if the loading was successful.
     */
    bool loadURLIntoTransport(URL const & inURL);
    /**
     Empties and resets the audio sources.
     */
    void emptyTransport();
    /**
     Sets the gain for the audio playback.
     
     @param inGain the gain value from 0.0 to 1.0;
     */
    void setGain(float inGain);
    
private:
    std::unique_ptr<TimeSliceThread> mThread;
    std::unique_ptr<AudioDeviceManager> mAudioDeviceManager;
    std::unique_ptr<AudioFormatManager> mFormatManager;
    std::unique_ptr<AudioSourcePlayer> mAudioSourcePlayer;
    std::unique_ptr<AudioTransportSource> mTransportSource;
    std::unique_ptr<AudioFormatReaderSource> mCurrentAudioFileSource;
    
    void timerCallback() override;
    /**
     Gets permission for audio recording and set audio i/o to default in- and ouput channels.
     */
    void initialiseDefaultDevice();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPlayer);
};
