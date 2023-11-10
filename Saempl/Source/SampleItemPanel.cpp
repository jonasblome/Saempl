/*
 ==============================================================================
 
 SampleItemPanel.cpp
 Created: 22 May 2023 9:46:19am
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleItemPanel.h"

SampleItemPanel::SampleItemPanel(SaemplAudioProcessor& inProcessor)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary())
{
    setSize(style->SAMPLE_ITEM_PANEL_WIDTH - style->PANEL_MARGIN / 2.0, style->SAMPLE_ITEM_PANEL_HEIGHT - style->PANEL_MARGIN / 2.0);
    setPanelComponents();
}

SampleItemPanel::~SampleItemPanel()
{
    mAudioPreviewPanel->removeChangeListener(this);
}

void SampleItemPanel::paint(Graphics& g)
{
    // Draw background
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), style->CORNER_SIZE_MEDIUM);
}

void SampleItemPanel::setPanelComponents()
{
    // Add sample editor component
    mSampleEditor = std::make_unique<SampleEditor>();
    
    int followTransportButtonHeight = 34;
    
    // Add play button component
    mStartStopButton = std::make_unique<BlomeImageButton>("Play/Stop", true);
    mStartStopButton->setImages(false,
                                true,
                                true,
                                ImageCache::getFromMemory(BinaryData::play_pause_FILL0_wght400_GRAD0_opsz24_png,
                                                          BinaryData::play_pause_FILL0_wght400_GRAD0_opsz24_pngSize),
                                style->BUTTON_IS_DEFAULT_ALPHA,
                                style->COLOUR_ACCENT_LIGHT,
                                Image(),
                                style->BUTTON_IS_OVER_ALPHA,
                                style->COLOUR_ACCENT_LIGHT,
                                Image(),
                                style->BUTTON_IS_DOWN_ALPHA,
                                style->COLOUR_ACCENT_LIGHT);
    mStartStopButton->setBounds(style->PANEL_MARGIN / 2.0,
                                style->PANEL_MARGIN / 2.0,
                                style->SAMPLE_CONTROL_WIDTH - style->PANEL_MARGIN / 2.0,
                                style->SAMPLE_CONTROL_WIDTH - style->PANEL_MARGIN / 2.0);
    mStartStopButton->setTooltip("Start/stop playback");
    mStartStopButton->onClick = [this] { mAudioPreviewPanel->startOrStop(); };
    addAndMakeVisible(*mStartStopButton);
    
    // Add follow transport button
    mFollowTransportButton = std::make_unique<BlomeImageButton>("Follow Transport", true);
    mFollowTransportButton->setImages(false,
                                      true,
                                      true,
                                      ImageCache::getFromMemory(BinaryData::trending_flat_FILL0_wght400_GRAD0_opsz24_png,
                                                                BinaryData::trending_flat_FILL0_wght400_GRAD0_opsz24_pngSize),
                                      style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                      style->COLOUR_ACCENT_LIGHT,
                                      Image(),
                                      style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                      style->COLOUR_ACCENT_LIGHT,
                                      Image(),
                                      style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                      style->COLOUR_ACCENT_LIGHT);
    mFollowTransportButton->setBounds(style->PANEL_MARGIN / 2.0,
                                      style->SAMPLE_CONTROL_WIDTH + style->PANEL_MARGIN / 2.0,
                                      style->SAMPLE_CONTROL_WIDTH - style->PANEL_MARGIN / 2.0,
                                      followTransportButtonHeight - style->PANEL_MARGIN / 2.0);
    mFollowTransportButton->setTooltip("Follow playback cursor");
    mFollowTransportButton->onClick = [this]
    {
        bool follows = !mAudioPreviewPanel->getFollowsTransport();
        mAudioPreviewPanel->setFollowsTransport(follows);
        mFollowTransportButton->setImages(false,
                                          true,
                                          true,
                                          ImageCache::getFromMemory(BinaryData::trending_flat_FILL0_wght400_GRAD0_opsz24_png,
                                                                    BinaryData::trending_flat_FILL0_wght400_GRAD0_opsz24_pngSize),
                                          follows 
                                          ? style->BUTTON_IS_DEFAULT_ALPHA
                                          : style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                          style->COLOUR_ACCENT_LIGHT,
                                          Image(),
                                          follows ?
                                          style->BUTTON_IS_OVER_ALPHA
                                          : style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                          style->COLOUR_ACCENT_LIGHT,
                                          Image(),
                                          follows ?
                                          style->BUTTON_IS_DOWN_ALPHA
                                          : style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                          style->COLOUR_ACCENT_LIGHT);
    };
    addAndMakeVisible(*mFollowTransportButton);
    
    // Add zoom slider
    mZoomSlider = std::make_unique<Slider>(Slider::LinearVertical, Slider::NoTextBox);
    mZoomSlider->setBounds(2,
                           style->SAMPLE_CONTROL_WIDTH + followTransportButtonHeight,
                           style->SAMPLE_CONTROL_WIDTH,
                           style->SAMPLE_CONTROL_HEIGHT - style->SAMPLE_CONTROL_WIDTH - followTransportButtonHeight);
    mZoomSlider->setRange(0, 1, 0);
    mZoomSlider->onValueChange = [this] { mAudioPreviewPanel->setZoomFactor(mZoomSlider->getValue()); };
    mZoomSlider->setSkewFactor(2);
    addAndMakeVisible(*mZoomSlider);
    
    // Add audio thumbnail component
    mAudioPreviewPanel = std::make_unique<AudioPreviewPanel>(currentProcessor, *mZoomSlider, *mSampleEditor);
    mAudioPreviewPanel->setTopLeftPosition(style->PANEL_MARGIN / 2.0 + style->SAMPLE_CONTROL_WIDTH, style->PANEL_MARGIN / 2.0);
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

bool SampleItemPanel::tryShowAudioResource(File inFile)
{
    // Load file into source
    if (inFile.exists() && !inFile.isDirectory() && isSupportedAudioFileFormat(inFile.getFileExtension()))
    {
        mAudioPreviewPanel->showAudioResource(URL(inFile));
        return true;
    }
    
    if (!inFile.exists())
    {
        mAudioPreviewPanel->emptyAudioResource();
        sampleLibrary.refresh();
    }
    
    return false;
}

void SampleItemPanel::startOrStopPlayback()
{
    mAudioPreviewPanel->startOrStop();
}
