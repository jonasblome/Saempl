/*
  ==============================================================================

    HeaderPanel.cpp
    Created: 27 May 2023 11:12:36am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "HeaderPanel.h"

HeaderPanel::HeaderPanel()
:   PanelBase()
{
    setSize(HEADER_PANEL_WIDTH - Blome_PanelMargin, HEADER_PANEL_HEIGHT - Blome_PanelMargin);
    setPanelComponents();
}

HeaderPanel::~HeaderPanel()
{
    
}

void HeaderPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Draw background
    g.setColour(BlomeColour_AccentColourMedium);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), Blome_PanelCornerSize);
    
    // Draw logo text
    const int logoWidth = 220;
    g.setColour(BlomeColour_LightGray);
    g.setFont(font_medium);
    g.drawFittedText("Blome Audio",
                     HEADER_PANEL_WIDTH - logoWidth - 20 - Blome_PanelMargin,
                     Blome_PanelMargin / 2.0,
                     logoWidth,
                     getHeight() - Blome_PanelMargin,
                     Justification::centredRight,
                     1);
}

void HeaderPanel::setPanelComponents()
{
    
}
