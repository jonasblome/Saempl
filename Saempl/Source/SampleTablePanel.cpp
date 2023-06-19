/*
  ==============================================================================

    SampleItemTablePanel.cpp
    Created: 18 Jun 2023 2:10:18pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleTablePanel.h"

SampleTablePanel::SampleTablePanel(SaemplAudioProcessor& inProcessor)
:   PanelBase(inProcessor)
{
    setSize(SAMPLE_ITEM_NAVIGATION_PANEL_WIDTH, SAMPLE_ITEM_NAVIGATION_PANEL_HEIGHT - PANEL_MARGIN * 1.5);
    setPanelComponents();
}

SampleTablePanel::~SampleTablePanel()
{
    
}

void SampleTablePanel::setPanelComponents()
{
    repaint();
}

void SampleTablePanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Set background
    g.setColour(BlomeColour_AccentColourMedium);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), PANEL_CORNER_SIZE);
}
