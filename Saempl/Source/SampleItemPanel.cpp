/*
  ==============================================================================

    SampleItemPanel.cpp
    Created: 22 May 2023 9:46:19am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleItemPanel.h"

SampleItemPanel::SampleItemPanel(TimeSliceThread& inThread)
:   PanelBase(),
    currentThread(inThread)
{
    setSize(SAMPLE_PANEL_WIDTH, SAMPLE_PANEL_HEIGHT);
    setPanelComponents();
}

SampleItemPanel::~SampleItemPanel()
{
    mAudioPreviewPanel->removeChangeListener(this);
}

void SampleItemPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Draw background
    g.setColour(BlomeColour_AccentColourDark);
    g.fillRoundedRectangle(Rectangle<float>(getLocalBounds().toFloat()), PanelCornerSize);
}

void SampleItemPanel::setPanelComponents()
{
    // Add sample editor component
    mSampleItemViewModel = std::make_unique<SampleItemViewModel>();
    
    int controlsHeight = SAMPLE_PANEL_HEIGHT - SAMPLE_PREVIEW_HEIGHT;
    
    // Add zoom slider
    mZoomSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mZoomSlider->setBounds(0, SAMPLE_PREVIEW_HEIGHT, getWidth() / 5.0, controlsHeight / 3.0);
    mZoomSlider->setRange(0, 1, 0);
    mZoomSlider->onValueChange = [this] { mAudioPreviewPanel->setZoomFactor(mZoomSlider->getValue()); };
    mZoomSlider->setSkewFactor(2);
    addAndMakeVisible(*mZoomSlider);

    // Add follow transport button
    mFollowTransportButton = std::make_unique<ToggleButton>("Follow Transport");
    mFollowTransportButton->setBounds(0, SAMPLE_PREVIEW_HEIGHT + controlsHeight / 3.0, controlsHeight / 3.0, controlsHeight / 3.0);
    mFollowTransportButton->onClick = [this] { mAudioPreviewPanel->setFollowsTransport(mFollowTransportButton->getToggleState()); };
    addAndMakeVisible(*mFollowTransportButton);
    
    // Add play button component
    mStartStopButton = std::make_unique<TextButton>("Play/Stop");
    mStartStopButton->setBounds(0, SAMPLE_PREVIEW_HEIGHT + 2 * controlsHeight / 3.0, 75, controlsHeight / 3.0);
    mStartStopButton->onClick = [this] { mAudioPreviewPanel->startOrStop(); };
    addAndMakeVisible(*mStartStopButton);
    
    // Add audio thumbnail component
    mAudioPreviewPanel = std::make_unique<AudioPreviewPanel>(currentThread, *mZoomSlider, *mSampleItemViewModel);
    mAudioPreviewPanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(mAudioPreviewPanel.get());
    mAudioPreviewPanel->addChangeListener(this);
    
    // Repaint panel
    repaint();
}

void SampleItemPanel::changeListenerCallback(ChangeBroadcaster* source)
{
    if(source == mAudioPreviewPanel.get())
    {
        mAudioPreviewPanel->showAudioResource();
    }
}

void SampleItemPanel::showAudioResource(URL inResource)
{
    mAudioPreviewPanel->showAudioResource(inResource);
}
