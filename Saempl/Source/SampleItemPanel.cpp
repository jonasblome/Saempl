/*
  ==============================================================================

    SampleItemPanel.cpp
    Created: 22 May 2023 9:46:19am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleItemPanel.h"

SampleItemPanel::SampleItemPanel(SaemplAudioProcessor& inProcessor)
:   PanelBase(),
    currentProcessor(inProcessor)
{
    setSize(SAMPLE_ITEM_PANEL_WIDTH - PANEL_MARGIN / 2.0, SAMPLE_ITEM_PANEL_HEIGHT - PANEL_MARGIN / 2.0);
    setPanelComponents();
}

SampleItemPanel::~SampleItemPanel()
{
    mAudioPreviewPanel->removeChangeListener(this);
}

void SampleItemPanel::paint(Graphics& g)
{
    // Draw background
    g.setColour(COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), CORNER_SIZE_MEDIUM);
}

void SampleItemPanel::setPanelComponents()
{
    // Add sample editor component
    mSampleEditor = std::make_unique<SampleEditor>();
    
    int controlsHeight = SAMPLE_CONTROL_HEIGHT;
    int followTransportButtonHeight = 25;
    
    // Add play button component
    mStartStopButton = std::make_unique<TextButton>(">");
    mStartStopButton->setBounds(PANEL_MARGIN / 2.0,
                                PANEL_MARGIN / 2.0,
                                BUTTON_SIZE_MEDIUM - PANEL_MARGIN / 2.0,
                                BUTTON_SIZE_MEDIUM - PANEL_MARGIN / 2.0);
    mStartStopButton->onClick = [this] { mAudioPreviewPanel->startOrStop(); };
    addAndMakeVisible(*mStartStopButton);
    
    // Add follow transport button
    mFollowTransportButton = std::make_unique<ToggleButton>("Follow Transport");
    mFollowTransportButton->setBounds(PANEL_MARGIN / 2.0,
                                      SAMPLE_CONTROL_WIDTH + PANEL_MARGIN / 2.0,
                                      SAMPLE_CONTROL_WIDTH - PANEL_MARGIN / 2.0,
                                      followTransportButtonHeight - PANEL_MARGIN / 2.0);
    mFollowTransportButton->onClick = [this] { mAudioPreviewPanel->setFollowsTransport(mFollowTransportButton->getToggleState()); };
    addAndMakeVisible(*mFollowTransportButton);
    
    // Add zoom slider
    mZoomSlider = std::make_unique<Slider>(Slider::LinearVertical, Slider::NoTextBox);
    mZoomSlider->setBounds(2,
                           SAMPLE_CONTROL_WIDTH + followTransportButtonHeight,
                           SAMPLE_CONTROL_WIDTH,
                           controlsHeight - SAMPLE_CONTROL_WIDTH - followTransportButtonHeight);
    mZoomSlider->setRange(0, 1, 0);
    mZoomSlider->onValueChange = [this] { mAudioPreviewPanel->setZoomFactor(mZoomSlider->getValue()); };
    mZoomSlider->setSkewFactor(2);
    addAndMakeVisible(*mZoomSlider);
    
    // Add audio thumbnail component
    mAudioPreviewPanel = std::make_unique<AudioPreviewPanel>(currentProcessor, *mZoomSlider, *mSampleEditor);
    mAudioPreviewPanel->setTopLeftPosition(PANEL_MARGIN / 2.0 + SAMPLE_CONTROL_WIDTH, PANEL_MARGIN / 2.0);
    addAndMakeVisible(mAudioPreviewPanel.get());
    mAudioPreviewPanel->addChangeListener(this);
    
    // Repaint panel
    repaint();
}

void SampleItemPanel::changeListenerCallback(ChangeBroadcaster* source)
{
    // If file was dropped into audio preview panel
    if(source == mAudioPreviewPanel.get())
    {
        mAudioPreviewPanel->showAudioResource();
    }
}

void SampleItemPanel::tryShowAudioResource(File inFile)
{
    // Load file into source
    if (inFile.exists() && !inFile.isDirectory() && isSupportedAudioFileFormat(inFile.getFileExtension()))
    {
        showAudioResource(URL(inFile));
    }
    else if (!inFile.exists())
    {
        showAudioResource(URL());
    }
}

void SampleItemPanel::showAudioResource(URL inResource)
{
    mAudioPreviewPanel->showAudioResource(inResource);
}
