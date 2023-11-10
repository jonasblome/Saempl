/*
 ==============================================================================
 
 CenterPanel.cpp
 Created: 27 May 2023 9:07:59pm
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "CenterPanel.h"

CenterPanel::CenterPanel(SaemplAudioProcessor& inProcessor)
:
PanelBase(inProcessor)
{
    setSize(style->CENTER_PANEL_WIDTH - style->PANEL_MARGIN / 2.0, style->CENTER_PANEL_HEIGHT - style->PANEL_MARGIN / 2.0);
    setPanelComponents();
}

CenterPanel::~CenterPanel()
{
    
}

void CenterPanel::paint(Graphics& g)
{
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds()
                           .removeFromBottom(style->BUTTON_SIZE_SMALL + style->PANEL_MARGIN)
                           .removeFromLeft(style->SAMPLE_NAVIGATION_PANEL_WIDTH)
                           .reduced(0, style->PANEL_MARGIN / 2.0)
                           .removeFromLeft(style->SAMPLE_NAVIGATION_PANEL_WIDTH - style->PANEL_MARGIN / 2.0)
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
    mToggleSampleItemPanelButton = std::make_unique<ToggleButton>("Toggle SampleItemPanel");
    mToggleSampleItemPanelButton->setToggleState(true, NotificationType::dontSendNotification);
    mToggleSampleItemPanelButton->setBounds(style->PANEL_MARGIN / 2.0,
                                            style->SAMPLE_NAVIGATION_PANEL_HEIGHT
                                            + style->SAMPLE_ITEM_PANEL_HEIGHT
                                            + style->PANEL_MARGIN / 2.0,
                                            style->BUTTON_SIZE_SMALL - style->PANEL_MARGIN,
                                            style->BUTTON_SIZE_SMALL - style->PANEL_MARGIN);
    mToggleSampleItemPanelButton->setTooltip("Toggle visibility of audio preview");
    mToggleSampleItemPanelButton->onClick = [this] { toggleSampleItemPanel(); };
    addAndMakeVisible(*mToggleSampleItemPanelButton);
    
    // Repaint panel
    repaint();
}

/**
 Toggles the visibility of the SampleItemPanel
 */
void CenterPanel::toggleSampleItemPanel()
{
    mSampleItemPanel->setVisible(!mSampleItemPanel->isVisible());
    
    if (mSampleItemPanel->isVisible())
    {
        mSampleNavigationPanel->setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH 
                                        - style->PANEL_MARGIN / 2.0,
                                        style->SAMPLE_NAVIGATION_PANEL_HEIGHT
                                        - style->PANEL_MARGIN / 2.0);
    }
    else
    {
        mSampleNavigationPanel->setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH
                                        - style->PANEL_MARGIN / 2.0,
                                        style->CENTER_PANEL_HEIGHT
                                        - style->BUTTON_SIZE_SMALL
                                        - style->PANEL_MARGIN * 1.5);
    }
    
    mSampleNavigationPanel->repaint();
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
