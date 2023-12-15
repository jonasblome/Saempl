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
                           .removeFromLeft(style->SAMPLE_NAVIGATION_PANEL_WIDTH)
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
    mSampleItemPanel->setTopLeftPosition(0, style->SAMPLE_NAVIGATION_PANEL_HEIGHT);
    addAndMakeVisible(*mSampleItemPanel);
    
    // Add panel for sample library views
    mSampleNavigationPanel = std::make_unique<SampleNavigationPanel>(currentProcessor, *mSampleItemPanel);
    mSampleNavigationPanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(*mSampleNavigationPanel);
    
    // Add panel for sample palette view
    mSamplePalettePanel = std::make_unique<SamplePalettePanel>(currentProcessor, *mSampleItemPanel);
    mSamplePalettePanel->setTopLeftPosition(style->SAMPLE_NAVIGATION_PANEL_WIDTH, 0);
    addAndMakeVisible(*mSamplePalettePanel);
    
    // Add toggle panel button
    mToggleSampleItemPanelButton = std::make_unique<ToggleButton>("ToggleSampleItemPanel");
    mToggleSampleItemPanelButton->setBounds(style->PANEL_MARGIN,
                                            style->SAMPLE_NAVIGATION_PANEL_HEIGHT
                                            + style->SAMPLE_ITEM_PANEL_HEIGHT
                                            + style->PANEL_MARGIN,
                                            style->BUTTON_SIZE_SMALL - style->PANEL_MARGIN,
                                            style->BUTTON_SIZE_SMALL - style->PANEL_MARGIN);
    mToggleSampleItemPanelButton->setTooltip("Toggle visibility of the audio player");
    mToggleSampleItemPanelButton->onClick = [this] { setSampleItemPanelVisibility(mToggleSampleItemPanelButton->getToggleState()); };
    mToggleSampleItemPanelButton->setToggleState(currentProcessor.getSampleItemPanelIsVisible(), NotificationType::dontSendNotification);
    addAndMakeVisible(*mToggleSampleItemPanelButton);
    
    // Set visibility of sample item panel
    setSampleItemPanelVisibility(currentProcessor.getSampleItemPanelIsVisible());
}

void CenterPanel::setSampleItemPanelVisibility(bool inShouldBeVisible)
{
    mSampleItemPanel->setVisible(inShouldBeVisible);
    currentProcessor.setSampleItemIsVisible(inShouldBeVisible);
    
    if (inShouldBeVisible)
    {
        mSampleNavigationPanel->setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH,
                                        style->SAMPLE_NAVIGATION_PANEL_HEIGHT);
    }
    else
    {
        mSampleNavigationPanel->setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH,
                                        style->CENTER_PANEL_HEIGHT
                                        - style->BUTTON_SIZE_SMALL
                                        - style->PANEL_MARGIN * 0.75);
    }
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
