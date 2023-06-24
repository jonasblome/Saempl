/*
  ==============================================================================

    HeaderPanel.cpp
    Created: 27 May 2023 11:12:36am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "HeaderPanel.h"

HeaderPanel::HeaderPanel(SaemplAudioProcessor& inProcessor)
:   PanelBase(inProcessor),
    currentProcessor(inProcessor)
{
    setSize(HEADER_PANEL_WIDTH - PANEL_MARGIN, HEADER_PANEL_HEIGHT - PANEL_MARGIN / 2.0);
    setPanelComponents();
}

HeaderPanel::~HeaderPanel()
{
    
}

void HeaderPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Draw background
    g.setColour(BlomeColour_AccentColourLight);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), Blome_PanelCornerSize);
    
    // Draw logo text
    const int logoWidth = 220;
    g.setColour(BlomeColour_AccentColourDark);
    g.setFont(font_medium);
    g.drawFittedText("Blome Audio",
                     HEADER_PANEL_WIDTH - logoWidth - 20 - PANEL_MARGIN,
                     PANEL_MARGIN / 2.0,
                     logoWidth,
                     getHeight() - PANEL_MARGIN,
                     Justification::centredRight,
                     1);
}

void HeaderPanel::setPanelComponents()
{
    // Add change directory button component
    mRefreshSampleLibrariesButton = std::make_unique<TextButton>("Refresh");
    mRefreshSampleLibrariesButton->setBounds(PANEL_MARGIN / 2.0,
                                            PANEL_MARGIN / 2.0,
                                            50,
                                            getHeight() - PANEL_MARGIN);
    mRefreshSampleLibrariesButton->onClick = [this] { currentProcessor.getSampleLibrary().refreshSampleLibrary(); };
    addAndMakeVisible(*mRefreshSampleLibrariesButton);
}
