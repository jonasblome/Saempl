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
     Shows the audio resource of the given file in the preview panel.
     
     @param inFile the file to show in the preview.
     */
    void tryShowAudioResource(File inFile);
    /**
     @see tryShowAudioResource(File inFile);
     */
    void showAudioResource(URL inResource);

private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleItemPanel)
    
    SaemplAudioProcessor& currentProcessor;
    std::unique_ptr<SampleEditor> mSampleEditor;
    std::unique_ptr<AudioPreviewPanel> mAudioPreviewPanel;
    std::unique_ptr<Slider> mZoomSlider;
    std::unique_ptr<ToggleButton> mFollowTransportButton;
    std::unique_ptr<TextButton> mStartStopButton;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
};
