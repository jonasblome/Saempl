/*
 ==============================================================================
 
 SampleItemPanel.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "PanelBase.h"
#include "AudioPreviewPanel.h"
#include "BlomeImageButton.h"

/**
 Holds the audio file preview panel and the AudioPlayer.
 
 Can preview audio files and play them.
 */
class SampleItemPanel
:
public PanelBase,
private ChangeListener
{
public:
    /**
     The constructor for the sample item panel.
     
     @param inProcessor the audio processor of the current plugin instance.
     */
    SampleItemPanel(SaemplAudioProcessor& inProcessor);
    ~SampleItemPanel();
    /**
     Tries to show the audio resource of the given file in the preview panel.
     If this fails the library gets refreshed.
     
     @param inFile the file to show in the preview.
     
     @returns whether the loading of the source was successful or not.
     */
    bool tryShowAudioResource(File const & inFile);
    /**
     Starts or stops the playback of the audio preview panel.
     */
    void startOrStopPlayback();
    bool keyPressed(KeyPress const & key) override;
    
private:
    SampleLibrary& sampleLibrary;
    std::unique_ptr<AudioPlayer> mAudioPlayer;
    std::unique_ptr<AudioPreviewPanel> mAudioPreviewPanel;
    std::unique_ptr<Slider> mZoomSlider;
    std::unique_ptr<BlomeImageButton> mFollowTransportButton;
    std::unique_ptr<BlomeImageButton> mStartStopButton;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void resizePanelComponents() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleItemPanel);
};
