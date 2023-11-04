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
    
    int followTransportButtonHeight = 34;
    
    // Add play button component
    mStartStopButton = std::make_unique<ImageButton>("Play/Stop");
    mStartStopButton->setImages(false,
                                true,
                                true,
                                ImageCache::getFromMemory(BinaryData::play_pause_FILL0_wght400_GRAD0_opsz24_png,
                                                          BinaryData::play_pause_FILL0_wght400_GRAD0_opsz24_pngSize),
                                1.0,
                                COLOUR_ACCENT_LIGHT,
                                Image(),
                                0.35,
                                Colour(),
                                Image(),
                                0.5,
                                Colour());
    mStartStopButton->setBounds(PANEL_MARGIN / 2.0,
                                PANEL_MARGIN / 2.0,
                                SAMPLE_CONTROL_WIDTH - PANEL_MARGIN / 2.0,
                                SAMPLE_CONTROL_WIDTH - PANEL_MARGIN / 2.0);
    mStartStopButton->onClick = [this] { mAudioPreviewPanel->startOrStop(); };
    addAndMakeVisible(*mStartStopButton);
    
    // Add follow transport button
    mFollowTransportButton = std::make_unique<ImageButton>("Follow Transport");
    mFollowTransportButton->setImages(false,
                                      true,
                                      true,
                                      ImageCache::getFromMemory(BinaryData::trending_flat_FILL0_wght400_GRAD0_opsz24_png,
                                                                BinaryData::trending_flat_FILL0_wght400_GRAD0_opsz24_pngSize),
                                      0.35,
                                      COLOUR_BLACK,
                                      Image(),
                                      0.25,
                                      Colour(),
                                      Image(),
                                      0.35,
                                      Colour());
    mFollowTransportButton->setBounds(PANEL_MARGIN / 2.0,
                                      SAMPLE_CONTROL_WIDTH + PANEL_MARGIN / 2.0,
                                      SAMPLE_CONTROL_WIDTH - PANEL_MARGIN / 2.0,
                                      followTransportButtonHeight - PANEL_MARGIN / 2.0);
    mFollowTransportButton->onClick = [this]
    {
        bool follows = !mAudioPreviewPanel->getFollowsTransport();
        mAudioPreviewPanel->setFollowsTransport(follows);
        mFollowTransportButton->setImages(false,
                                          true,
                                          true,
                                          ImageCache::getFromMemory(BinaryData::trending_flat_FILL0_wght400_GRAD0_opsz24_png,
                                                                    BinaryData::trending_flat_FILL0_wght400_GRAD0_opsz24_pngSize),
                                          follows ? 1.0 : 0.35,
                                          follows ? COLOUR_ACCENT_LIGHT : COLOUR_BLACK,
                                          Image(),
                                          follows ? 0.35 : 0.25,
                                          Colour(),
                                          Image(),
                                          follows ? 0.5 : 0.35,
                                          Colour());
    };
    addAndMakeVisible(*mFollowTransportButton);
    
    // Add zoom slider
    mZoomSlider = std::make_unique<Slider>(Slider::LinearVertical, Slider::NoTextBox);
    mZoomSlider->setBounds(2,
                           SAMPLE_CONTROL_WIDTH + followTransportButtonHeight,
                           SAMPLE_CONTROL_WIDTH,
                           SAMPLE_CONTROL_HEIGHT - SAMPLE_CONTROL_WIDTH - followTransportButtonHeight);
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
        currentProcessor.getSampleLibrary().refresh();
    }
    
    return false;
}
