/*
 ==============================================================================
 
 SampleItemPanel.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleItemPanel.h"

SampleItemPanel::SampleItemPanel(SaemplAudioProcessor& inProcessor)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary())
{
    setSize(style->SAMPLE_ITEM_PANEL_WIDTH, style->SAMPLE_ITEM_PANEL_HEIGHT);
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
    g.fillRoundedRectangle(getLocalBounds()
                           .toFloat()
                           .withTrimmedLeft(style->PANEL_MARGIN * 0.5)
                           .withTrimmedTop(style->PANEL_MARGIN * 0.25)
                           .withTrimmedRight(style->PANEL_MARGIN * 0.25),
                           style->CORNER_SIZE_MEDIUM);
}

void SampleItemPanel::setPanelComponents()
{
    // Add audio player component
    mAudioPlayer = std::make_unique<AudioPlayer>();
    
    int followTransportButtonHeight = 34;
    
    // Add play button component
    mStartStopButton = std::make_unique<BlomeImageButton>("Play/Stop", true);
    mStartStopButton->setImages(false,
                                true,
                                true,
                                ImageCache::getFromMemory(BinaryData::play_pause_FILL0_wght400_GRAD0_opsz24_png,
                                                          BinaryData::play_pause_FILL0_wght400_GRAD0_opsz24_pngSize),
                                style->BUTTON_IS_DEFAULT_ALPHA,
                                style->COLOUR_SYMBOL_BUTTON,
                                Image(),
                                style->BUTTON_IS_OVER_ALPHA,
                                style->COLOUR_SYMBOL_BUTTON,
                                Image(),
                                style->BUTTON_IS_DOWN_ALPHA,
                                style->COLOUR_SYMBOL_BUTTON);
    mStartStopButton->setBounds(style->PANEL_MARGIN,
                                style->PANEL_MARGIN * 0.75,
                                style->SAMPLE_CONTROL_WIDTH - style->PANEL_MARGIN / 2.0,
                                style->SAMPLE_CONTROL_WIDTH - style->PANEL_MARGIN / 2.0);
    mStartStopButton->setTooltip("Start/stop playback (Space)");
    mStartStopButton->onClick = [this] { mAudioPreviewPanel->startOrStop(); };
    addAndMakeVisible(*mStartStopButton);
    
    // Add follow transport button
    mFollowTransportButton = std::make_unique<BlomeImageButton>("Follow Transport", true);
    bool initialFollows = currentProcessor.getFollowAudioPlayhead();
    mFollowTransportButton->setImages(false,
                                      true,
                                      true,
                                      ImageCache::getFromMemory(BinaryData::trending_flat_FILL0_wght400_GRAD0_opsz24_png,
                                                                BinaryData::trending_flat_FILL0_wght400_GRAD0_opsz24_pngSize),
                                      initialFollows
                                      ? style->BUTTON_IS_DEFAULT_ALPHA
                                      : style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                      style->COLOUR_SYMBOL_BUTTON,
                                      Image(),
                                      initialFollows ?
                                      style->BUTTON_IS_OVER_ALPHA
                                      : style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                      style->COLOUR_SYMBOL_BUTTON,
                                      Image(),
                                      initialFollows ?
                                      style->BUTTON_IS_DOWN_ALPHA
                                      : style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                      style->COLOUR_SYMBOL_BUTTON);
    mFollowTransportButton->setBounds(style->PANEL_MARGIN,
                                      style->SAMPLE_CONTROL_WIDTH + style->PANEL_MARGIN * 0.75,
                                      style->SAMPLE_CONTROL_WIDTH - style->PANEL_MARGIN / 2.0,
                                      followTransportButtonHeight - style->PANEL_MARGIN / 2.0);
    mFollowTransportButton->setTooltip("Follow playback cursor");
    mFollowTransportButton->onClick = [this]
    {
        bool follows = !mAudioPreviewPanel->getFollowsTransport();
        currentProcessor.setFollowAudioPlayhead(follows);
        mAudioPreviewPanel->setFollowsTransport(follows);
        mFollowTransportButton->setImages(false,
                                          true,
                                          true,
                                          ImageCache::getFromMemory(BinaryData::trending_flat_FILL0_wght400_GRAD0_opsz24_png,
                                                                    BinaryData::trending_flat_FILL0_wght400_GRAD0_opsz24_pngSize),
                                          follows 
                                          ? style->BUTTON_IS_DEFAULT_ALPHA
                                          : style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                          style->COLOUR_SYMBOL_BUTTON,
                                          Image(),
                                          follows ?
                                          style->BUTTON_IS_OVER_ALPHA
                                          : style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                          style->COLOUR_SYMBOL_BUTTON,
                                          Image(),
                                          follows ?
                                          style->BUTTON_IS_DOWN_ALPHA
                                          : style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                          style->COLOUR_SYMBOL_BUTTON);
    };
    addAndMakeVisible(*mFollowTransportButton);
    
    // Add zoom slider
    mZoomSlider = std::make_unique<Slider>(Slider::LinearVertical, Slider::NoTextBox);
    mZoomSlider->setBounds(style->PANEL_MARGIN,
                           style->SAMPLE_CONTROL_WIDTH + followTransportButtonHeight,
                           style->SAMPLE_CONTROL_WIDTH,
                           style->SAMPLE_CONTROL_HEIGHT - style->SAMPLE_CONTROL_WIDTH - followTransportButtonHeight);
    mZoomSlider->setRange(0, 1, 0);
    mZoomSlider->onValueChange = [this] { mAudioPreviewPanel->setZoomFactor(mZoomSlider->getValue()); };
    mZoomSlider->setSkewFactor(1.3);
    mZoomSlider->setTooltip("Zooms into the waveform of the current sample");
    addAndMakeVisible(*mZoomSlider);
    
    // Add audio thumbnail component
    mAudioPreviewPanel = std::make_unique<AudioPreviewPanel>(currentProcessor, *mZoomSlider, *mAudioPlayer);
    mAudioPreviewPanel->addChangeListener(this);
    mAudioPreviewPanel->setFollowsTransport(currentProcessor.getFollowAudioPlayhead());
    addAndMakeVisible(mAudioPreviewPanel.get());
    
    resizePanelComponents();
}

void SampleItemPanel::changeListenerCallback(ChangeBroadcaster* source)
{
    // If file was dropped into audio preview panel
    if (source == &*mAudioPreviewPanel)
    {
        mAudioPreviewPanel->showAudioResource();
    }
}

bool SampleItemPanel::tryShowAudioResource(File const & inFile)
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
    
    AlertWindow::showAsync(MessageBoxOptions()
                           .withIconType(MessageBoxIconType::NoIcon)
                           .withTitle("File not available!")
                           .withMessage("This file has probably been externally deleted and was removed from the list of available samples.")
                           .withButton("OK"),
                           nullptr);
    
    return false;
}

void SampleItemPanel::startOrStopPlayback()
{
    mAudioPreviewPanel->startOrStop();
}

void SampleItemPanel::resizePanelComponents()
{
    if (mAudioPreviewPanel != nullptr)
    {
        mAudioPreviewPanel->setBounds(style->PANEL_MARGIN + style->SAMPLE_CONTROL_WIDTH,
                                      style->PANEL_MARGIN * 0.5,
                                      getWidth() - style->SAMPLE_CONTROL_WIDTH - style->PANEL_MARGIN * 1.75,
                                      getHeight());
    }
}
