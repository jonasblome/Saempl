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
#include "SampleEditor.h"

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
    void setPanelStyle();
    void showAudioResource(URL resource);

private:
    std::unique_ptr<SampleEditor> mSampleEditor;
    
    // Audio player elements
    std::unique_ptr<AudioDeviceManager> mAudioDeviceManager;
    AudioFormatManager mFormatManager;
    TimeSliceThread& audioReadaheadThread;
    AudioSourcePlayer mAudioSourcePlayer;
    AudioTransportSource mTransportSource;
    std::unique_ptr<AudioFormatReaderSource> mCurrentAudioFileSource;
    std::unique_ptr<AudioPreviewPanel> mAudioPreviewPanel;
    std::unique_ptr<Slider> mZoomSlider;
    std::unique_ptr<ToggleButton> mFollowTransportButton;
    std::unique_ptr<TextButton> mStartStopButton;
    URL mCurrentAudioFile;
    
    // Methods
    bool loadURLIntoTransport(const URL& audioURL);
    void startOrStop();
    void updateFollowTransportState();
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleItemPanel)
};
