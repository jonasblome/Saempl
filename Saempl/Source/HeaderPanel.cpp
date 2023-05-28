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
    setSize(HEADER_PANEL_WIDTH, HEADER_PANEL_HEIGHT);
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
    g.fillRoundedRectangle(Rectangle<float>(getLocalBounds().toFloat()), PanelCornerSize);
    
    // Draw logo text
    const int logoWidth = 220;
    g.setColour(BlomeColour_LightGray);
    g.setFont(font_medium);
    g.drawFittedText("Blome Audio",
                     HEADER_PANEL_WIDTH - logoWidth - 20,
                     0,
                     logoWidth,
                     getHeight(),
                     Justification::centredRight,
                     1);
}

void HeaderPanel::setPanelComponents()
{
    
}
