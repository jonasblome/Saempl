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

class SampleItemPanel
:   public PanelBase,
    private ChangeListener
{
public:
    // Constructors
    SampleItemPanel(SaemplAudioProcessor& inProcessor);
    ~SampleItemPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void tryShowAudioResource(File inFile);
    void showAudioResource(URL inResource);

private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleItemPanel)
    
    // Fields
    SaemplAudioProcessor& currentProcessor;
    std::unique_ptr<SampleEditor> mSampleEditor;
    std::unique_ptr<AudioPreviewPanel> mAudioPreviewPanel;
    std::unique_ptr<Slider> mZoomSlider;
    std::unique_ptr<ToggleButton> mFollowTransportButton;
    std::unique_ptr<TextButton> mStartStopButton;
    
    // Methods
    void changeListenerCallback (ChangeBroadcaster* source) override;
};
