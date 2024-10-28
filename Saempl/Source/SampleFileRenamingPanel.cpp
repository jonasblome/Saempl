/*
  ==============================================================================

    SampleFileRenamingPanel.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileRenamingPanel.h"

SampleFileRenamingPanel::SampleFileRenamingPanel(SaemplAudioProcessor & inProcessor,
                                                 String inFilePath)
:
PanelBase(inProcessor),
originalFilePath(inFilePath)
{
    setSize(style->RENAMING_PANEL_WIDTH, style->RENAMING_PANEL_HEIGHT);
    setPanelComponents();
}

SampleFileRenamingPanel::~SampleFileRenamingPanel()
{
    
}

void SampleFileRenamingPanel::paint(Graphics& g)
{
    // Draw background
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(),
                           style->CORNER_SIZE_MEDIUM);
}

void SampleFileRenamingPanel::setPanelComponents()
{
    
}
