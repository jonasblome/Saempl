/*
 ==============================================================================
 
 CentrePanel.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "CentrePanel.h"

CentrePanel::CentrePanel(SaemplAudioProcessor& inProcessor)
:
PanelBase(inProcessor)
{
    setSize(style->CENTRE_PANEL_WIDTH, style->CENTRE_PANEL_HEIGHT);
    setPanelComponents();
}

CentrePanel::~CentrePanel()
{
    
}

void CentrePanel::paint(Graphics& g)
{
    // Draw sample item toggle area background
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds()
                           .removeFromBottom(style->BUTTON_SIZE_SMALL + style->PANEL_MARGIN * 0.75)
                           .removeFromLeft(getWidth() - style->SAMPLE_FAVOURITES_PANEL_WIDTH)
                           .withTrimmedLeft(style->PANEL_MARGIN * 0.5)
                           .withTrimmedTop(style->PANEL_MARGIN * 0.25)
                           .withTrimmedRight(style->PANEL_MARGIN * 0.25)
                           .withTrimmedBottom(style->PANEL_MARGIN * 0.5)
                           .toFloat(),
                           style->CORNER_SIZE_MEDIUM);
}

void CentrePanel::setPanelComponents()
{
    // Add audio player
    mAudioPlayer = std::make_unique<AudioPlayer>();
    mAudioPlayer->selectOutputDevice(currentProcessor.getOutputDevice());
    mAudioPlayer->setVolumeIsNormalised(currentProcessor.getVolumeIsNormalised());
    
    // Add panel for sample item view
    mSampleItemPanel = std::make_unique<SampleItemPanel>(currentProcessor);
    addAndMakeVisible(*mSampleItemPanel);
    
    // Add panel for sample library views
    mSampleNavigationPanel = std::make_unique<SampleNavigationPanel>(currentProcessor, *mSampleItemPanel, *mAudioPlayer);
    addAndMakeVisible(*mSampleNavigationPanel);
    
    // Add panel for sample favourites view
    mSampleFavouritesPanel = std::make_unique<SampleFavouritesPanel>(currentProcessor, *mSampleItemPanel, *mAudioPlayer);
    addAndMakeVisible(*mSampleFavouritesPanel);
    
    // Add toggle panel button
    mToggleSampleItemPanelButton = std::make_unique<ToggleButton>("ToggleSampleItemPanel");
    mToggleSampleItemPanelButton->setTooltip("Toggle visibility of the audio player (P)");
    mToggleSampleItemPanelButton->onClick = [this] { setSampleItemPanelVisibility(mToggleSampleItemPanelButton->getToggleState()); };
    mToggleSampleItemPanelButton->setToggleState(currentProcessor.getSampleItemPanelIsVisible(), NotificationType::dontSendNotification);
    addAndMakeVisible(*mToggleSampleItemPanelButton);
    
    // Set visibility of sample item panel
    setSampleItemPanelVisibility(currentProcessor.getSampleItemPanelIsVisible());
    resizePanelComponents();
}

void CentrePanel::setSampleItemPanelVisibility(bool inShouldBeVisible)
{
    mSampleItemPanel->setVisible(inShouldBeVisible);
    currentProcessor.setSampleItemIsVisible(inShouldBeVisible);
    resizePanelComponents();
}

void CentrePanel::setActiveNavigationPanel(NavigationPanelType inPanelType)
{
    mSampleNavigationPanel->setActiveNavigationPanel(inPanelType);
}

void CentrePanel::selectRandomSample()
{
    mSampleNavigationPanel->selectRandomSample();
}

void CentrePanel::resizePanelComponents()
{
    if (mSampleNavigationPanel != nullptr)
    {
        if (currentProcessor.getSampleItemPanelIsVisible())
        {
            mSampleNavigationPanel->setBounds(0,
                                              0,
                                              getWidth() - style->SAMPLE_FAVOURITES_PANEL_WIDTH,
                                              getHeight()
                                              - style->SAMPLE_ITEM_PANEL_HEIGHT
                                              - style->BUTTON_SIZE_SMALL
                                              - style->PANEL_MARGIN);
        }
        else
        {
            mSampleNavigationPanel->setBounds(0,
                                              0,
                                              getWidth() - style->SAMPLE_FAVOURITES_PANEL_WIDTH,
                                              getHeight()
                                              - style->BUTTON_SIZE_SMALL
                                              - style->PANEL_MARGIN * 0.75);
        }
    }
    
    if (mSampleItemPanel != nullptr)
    {
        mSampleItemPanel->setBounds(0,
                                    getHeight() - style->SAMPLE_ITEM_PANEL_HEIGHT
                                    - style->BUTTON_SIZE_SMALL
                                    - style->PANEL_MARGIN,
                                    getWidth() - style->SAMPLE_FAVOURITES_PANEL_WIDTH,
                                    style->SAMPLE_ITEM_PANEL_HEIGHT);
    }
    
    if (mSampleFavouritesPanel != nullptr)
    {
        mSampleFavouritesPanel->setBounds(getWidth() - style->SAMPLE_FAVOURITES_PANEL_WIDTH,
                                       0,
                                       style->SAMPLE_FAVOURITES_PANEL_WIDTH,
                                       getHeight());
    }
    
    if (mToggleSampleItemPanelButton != nullptr)
    {
        mToggleSampleItemPanelButton->setBounds(style->PANEL_MARGIN,
                                                getHeight() - style->BUTTON_SIZE_SMALL,
                                                style->BUTTON_SIZE_SMALL - style->PANEL_MARGIN,
                                                style->BUTTON_SIZE_SMALL - style->PANEL_MARGIN);
    }
}

bool CentrePanel::keyPressed(KeyPress const & key)
{
    bool pressWasHandled = false;
    
    int keyCode = key.getKeyCode();
    
    if (key.getKeyCode() == 76) // L
    {
        stopSelectedSample();
        return true;
    }
    else if (keyCode == 80) // P
    {
        mToggleSampleItemPanelButton->triggerClick();
        return true;
    }
    else if (mSampleNavigationPanel->keyPressed(key)
             || mSampleItemPanel->keyPressed(key))
    {
        pressWasHandled = true;
    }
    
    return pressWasHandled;
}

void CentrePanel::setGain(float inGain)
{
    currentProcessor.setOutputGain(inGain);
    mAudioPlayer->setGain(inGain);
    mSampleItemPanel->setGain(inGain);
}

void CentrePanel::toggleVolumeIsNormalised()
{
    bool volumeIsNormalised = !currentProcessor.getVolumeIsNormalised();
    currentProcessor.setVolumeIsNormalised(volumeIsNormalised);
    mAudioPlayer->setVolumeIsNormalised(volumeIsNormalised);
    mSampleItemPanel->setVolumeIsNormalised(volumeIsNormalised);
}

void CentrePanel::selectOutputDevice(String inDeviceName)
{
    currentProcessor.setOutputDevice(inDeviceName);
    mAudioPlayer->selectOutputDevice(inDeviceName);
    mSampleItemPanel->selectOutputDevice(inDeviceName);
}

void CentrePanel::stopSelectedSample()
{
    mAudioPlayer->stop();
}
