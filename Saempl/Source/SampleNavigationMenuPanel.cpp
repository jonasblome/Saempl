/*
  ==============================================================================

    SampleItemNavigationMenuPanel.cpp
    Created: 18 Jun 2023 6:01:12pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleNavigationMenuPanel.h"

SampleNavigationMenuPanel::SampleNavigationMenuPanel(SaemplAudioProcessor& inProcessor, SampleNavigationPanel& inSampleItemNavigationPanel)
:   PanelBase(inProcessor),
    linkedSampleItemNavigationPanel(inSampleItemNavigationPanel)
{
    setSize(SAMPLE_ITEM_NAVIGATION_MENU_PANEL_WIDTH, SAMPLE_ITEM_NAVIGATION_MENU_PANEL_HEIGHT);
    setPanelComponents();
}

SampleNavigationMenuPanel::~SampleNavigationMenuPanel()
{
    
}

void SampleNavigationMenuPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Set background
    g.setColour(BlomeColour_AccentColourLight);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), PANEL_CORNER_SIZE);
}

void SampleNavigationMenuPanel::setPanelComponents()
{
    // Add toggle button for sample item filter panel
    mToggleSampleItemFilterPanelButton = std::make_unique<ToggleButton>("Toggle SampleItemFilterPanel");
    mToggleSampleItemFilterPanelButton->setToggleState(false, NotificationType::dontSendNotification);
    mToggleSampleItemFilterPanelButton->setBounds(PANEL_MARGIN / 2.0,
                                                  PANEL_MARGIN / 2.0,
                                                  getHeight() - PANEL_MARGIN,
                                                  getHeight() - PANEL_MARGIN);
    mToggleSampleItemFilterPanelButton->onClick = [this] { linkedSampleItemNavigationPanel.toggleSampleItemFilterPanel(); };
    addAndMakeVisible(*mToggleSampleItemFilterPanelButton);
    
    // Repaint panel
    repaint();
}
