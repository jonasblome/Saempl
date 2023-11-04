/*
  ==============================================================================

    SampleItemPanel.h
    Created: 22 May 2023 9:46:19am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "AudioPreviewPanel.h"

/**
 Holds the audio file preview panel and the \ref SampleEditor
 
 Implements:
 \ref PanelBase
 \ref juce::ChangeListener
 Can preview audio files and play them.
 */
class SampleItemPanel
:   public PanelBase,
    private ChangeListener
{
public:
    /**
     The constructor for the sample item panel.
     
     @param inProcessor the audio processor of the current plugin instance.
     */
    SampleItemPanel(SaemplAudioProcessor& inProcessor);
    ~SampleItemPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    /**
     Tries to show the audio resource of the given file in the preview panel.
     If this fails the library gets refreshed.
     
     @param inFile the file to show in the preview.
     
     @returns whether the loading of the source was successful or not.
     */
    bool tryShowAudioResource(File inFile);

private:
    SaemplAudioProcessor& currentProcessor;
    std::unique_ptr<SampleEditor> mSampleEditor;
    std::unique_ptr<AudioPreviewPanel> mAudioPreviewPanel;
    std::unique_ptr<Slider> mZoomSlider;
    std::unique_ptr<ImageButton> mFollowTransportButton;
    std::unique_ptr<ImageButton> mStartStopButton;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
};
