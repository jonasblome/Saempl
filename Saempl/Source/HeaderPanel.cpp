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
}

HeaderPanel::~HeaderPanel()
{
    
}

void HeaderPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Draw panel background
    g.fillAll(BlomeColour_DarkRed);
    
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
