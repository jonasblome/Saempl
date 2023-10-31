/*
  ==============================================================================

    CenterPanel.cpp
    Created: 27 May 2023 9:07:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "CenterPanel.h"

CenterPanel::CenterPanel(SaemplAudioProcessor& inProcessor)
:   PanelBase(),
    currentProcessor(inProcessor)
{
    setSize(CENTER_PANEL_WIDTH - PANEL_MARGIN / 2.0, CENTER_PANEL_HEIGHT - PANEL_MARGIN / 2.0);
    setPanelComponents();
}

CenterPanel::~CenterPanel()
{
    
}

void CenterPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
}

void CenterPanel::setPanelComponents()
{
    // Add panel for sample item view
    mSampleItemPanel = std::make_unique<SampleItemPanel>(currentProcessor);
    mSampleItemPanel->setTopLeftPosition(0, SAMPLE_NAVIGATION_PANEL_HEIGHT);
    addAndMakeVisible(*mSampleItemPanel);
    
    // Add panel for database views
    mSampleNavigationPanel = std::make_unique<SampleNavigationPanel>(currentProcessor, *mSampleItemPanel);
    mSampleNavigationPanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(*mSampleNavigationPanel);
    
    // Add toggle panel button
    mToggleSampleItemPanelButton = std::make_unique<ToggleButton>("Toggle SampleItemPanel");
    mToggleSampleItemPanelButton->setToggleState(true, NotificationType::dontSendNotification);
    mToggleSampleItemPanelButton->setBounds(0,
                                            SAMPLE_NAVIGATION_PANEL_HEIGHT + SAMPLE_ITEM_PANEL_HEIGHT,
                                            BUTTON_SIZE_SMALL - PANEL_MARGIN,
                                            BUTTON_SIZE_SMALL - PANEL_MARGIN);
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
    
    if(mSampleItemPanel->isVisible())
    {
        mSampleNavigationPanel->setSize(SAMPLE_NAVIGATION_PANEL_WIDTH - PANEL_MARGIN,
                                        SAMPLE_NAVIGATION_PANEL_HEIGHT - PANEL_MARGIN / 2.0);
    }
    else
    {
        mSampleNavigationPanel->setSize(SAMPLE_NAVIGATION_PANEL_WIDTH - PANEL_MARGIN,
                                        CENTER_PANEL_HEIGHT - BUTTON_SIZE_SMALL - PANEL_MARGIN / 2.0);
    }
    
    mSampleNavigationPanel->repaint();
}

void CenterPanel::showNavigationPanel(NavigationPanelType inPanelType)
{
    mSampleNavigationPanel->showNavigationPanel(inPanelType);
}
