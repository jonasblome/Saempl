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

/**
 Handles audio playback of the \ref AudioPlayer and later audio editing like time stretching and pitch shifting.
 */
class SampleEditor
{
public:
    SampleEditor();
    ~SampleEditor();
    /**
     @returns the player's audio format manager.
     */
    AudioFormatManager& getAudioFormatManager();
    /**
     Starts or stops the audio playback.
     */
    void startOrStopAudio();
    /**
     Sets the readhead position of the audio player.
     
     @param position the position in seconds.
     */
    void setAudioReadheadPosition(double position);
    /**
     Starts audio playback.
     */
    void startAudio();
    /**
     @returns if the player currently plays back audio.
     */
    bool isPlaying();
    /**
     @returns the current readhead position of the audio player.
     */
    double getCurrentReadheadPosition();
    /**
     Stops the audio playback.
     */
    void stopAudio();
    /**
     Loads the given URL into the audio transport source.
     
     @param audioURL the URL to load into the transport source.
     
     @returns if the loading was successful.
     */
    bool loadURLIntoTransport(URL const & audioURL);
    /**
     Empties and resets the audio player's audio sources.
     */
    void emptyTransport();
    
private:
    std::unique_ptr<AudioPlayer> mAudioPlayer;
};
