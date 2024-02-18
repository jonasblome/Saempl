/*
 ==============================================================================
 
 CenterPanel.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "CenterPanel.h"

CenterPanel::CenterPanel(SaemplAudioProcessor& inProcessor)
:
PanelBase(inProcessor)
{
    setSize(style->CENTER_PANEL_WIDTH, style->CENTER_PANEL_HEIGHT);
    setPanelComponents();
    setWantsKeyboardFocus(true);
}

CenterPanel::~CenterPanel()
{
    
}

void CenterPanel::paint(Graphics& g)
{
    // Draw sample item toggle area background
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds()
                           .removeFromBottom(style->BUTTON_SIZE_SMALL + style->PANEL_MARGIN * 0.75)
                           .removeFromLeft(getWidth() - style->SAMPLE_PALETTE_PANEL_WIDTH)
                           .withTrimmedLeft(style->PANEL_MARGIN * 0.5)
                           .withTrimmedTop(style->PANEL_MARGIN * 0.25)
                           .withTrimmedRight(style->PANEL_MARGIN * 0.25)
                           .withTrimmedBottom(style->PANEL_MARGIN * 0.5)
                           .toFloat(),
                           style->CORNER_SIZE_MEDIUM);
}

void CenterPanel::setPanelComponents()
{
    // Add panel for sample item view
    mSampleItemPanel = std::make_unique<SampleItemPanel>(currentProcessor);
    addAndMakeVisible(*mSampleItemPanel);
    
    // Add panel for sample library views
    mSampleNavigationPanel = std::make_unique<SampleNavigationPanel>(currentProcessor, *mSampleItemPanel);
    addAndMakeVisible(*mSampleNavigationPanel);
    
    // Add panel for sample palette view
    mSamplePalettePanel = std::make_unique<SamplePalettePanel>(currentProcessor, *mSampleItemPanel);
    addAndMakeVisible(*mSamplePalettePanel);
    
    // Add toggle panel button
    mToggleSampleItemPanelButton = std::make_unique<ToggleButton>("ToggleSampleItemPanel");
    mToggleSampleItemPanelButton->setTooltip("Toggle visibility of the audio player (T)");
    mToggleSampleItemPanelButton->onClick = [this] { setSampleItemPanelVisibility(mToggleSampleItemPanelButton->getToggleState()); };
    mToggleSampleItemPanelButton->setToggleState(currentProcessor.getSampleItemPanelIsVisible(), NotificationType::dontSendNotification);
    addAndMakeVisible(*mToggleSampleItemPanelButton);
    
    // Set visibility of sample item panel
    setSampleItemPanelVisibility(currentProcessor.getSampleItemPanelIsVisible());
    resizePanelComponents();
}

void CenterPanel::setSampleItemPanelVisibility(bool inShouldBeVisible)
{
    mSampleItemPanel->setVisible(inShouldBeVisible);
    currentProcessor.setSampleItemIsVisible(inShouldBeVisible);
    resizePanelComponents();
}

void CenterPanel::setActiveNavigationPanel(NavigationPanelType inPanelType)
{
    mSampleNavigationPanel->setActiveNavigationPanel(inPanelType);
}

void CenterPanel::selectRandomSample()
{
    mSampleNavigationPanel->selectRandomSample();
}

void CenterPanel::playCurrentAudio()
{
    mSampleItemPanel->startOrStopPlayback();
}

void CenterPanel::resizePanelComponents()
{
    if (mSampleNavigationPanel != nullptr)
    {
        if (currentProcessor.getSampleItemPanelIsVisible())
        {
            mSampleNavigationPanel->setBounds(0,
                                              0,
                                              getWidth() - style->SAMPLE_PALETTE_PANEL_WIDTH,
                                              getHeight()
                                              - style->SAMPLE_ITEM_PANEL_HEIGHT
                                              - style->BUTTON_SIZE_SMALL
                                              - style->PANEL_MARGIN);
        }
        else
        {
            mSampleNavigationPanel->setBounds(0,
                                              0,
                                              getWidth() - style->SAMPLE_PALETTE_PANEL_WIDTH,
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
                                    getWidth() - style->SAMPLE_PALETTE_PANEL_WIDTH,
                                    style->SAMPLE_ITEM_PANEL_HEIGHT);
    }
    
    if (mSamplePalettePanel != nullptr)
    {
        mSamplePalettePanel->setBounds(getWidth() - style->SAMPLE_PALETTE_PANEL_WIDTH,
                                       0,
                                       style->SAMPLE_PALETTE_PANEL_WIDTH,
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

bool CenterPanel::keyPressed(KeyPress const & key)
{
    int keyCode = key.getKeyCode();
    
    if (key.isKeyCode(KeyPress::spaceKey))
    {
        playCurrentAudio();
        return true;
    }
    else if (keyCode == 84) // T
    {
        mToggleSampleItemPanelButton->triggerClick();
        return true;
    }
    
    return false;
}
