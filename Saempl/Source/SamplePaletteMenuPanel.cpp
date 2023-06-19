/*
  ==============================================================================

    SampleItemPaletteMenuPanel.cpp
    Created: 18 Jun 2023 6:29:52pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SamplePaletteMenuPanel.h"

SamplePaletteMenuPanel::SamplePaletteMenuPanel(SaemplAudioProcessor& inProcessor, SamplePalettePanel& inSampleItemPalettePanel)
:   PanelBase(inProcessor),
    linkedSampleItemPalettePanel(inSampleItemPalettePanel)
{
    setSize(SAMPLE_ITEM_PALETTE_MENU_PANEL_WIDTH - PANEL_MARGIN, SAMPLE_ITEM_PALETTE_MENU_PANEL_HEIGHT);
    setPanelComponents();
}

SamplePaletteMenuPanel::~SamplePaletteMenuPanel()
{
    
}

void SamplePaletteMenuPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Set background
    g.setColour(BlomeColour_AccentColourLight);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), PANEL_CORNER_SIZE);
}

void SamplePaletteMenuPanel::setPanelComponents()
{
    // Repaint panel
    repaint();
}
