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
#include "SampleItemViewModel.h"

class SampleItemPanel
:   public PanelBase,
    private ChangeListener
{
public:
    // Constructors
    SampleItemPanel(TimeSliceThread& inThread);
    ~SampleItemPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void showAudioResource(URL inResource);

private:
    // Fields
    TimeSliceThread& currentThread;
    std::unique_ptr<SampleItemViewModel> mSampleItemViewModel;
    std::unique_ptr<AudioPreviewPanel> mAudioPreviewPanel;
    std::unique_ptr<Slider> mZoomSlider;
    std::unique_ptr<ToggleButton> mFollowTransportButton;
    std::unique_ptr<TextButton> mStartStopButton;
    
    // Methods
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleItemPanel)
};
