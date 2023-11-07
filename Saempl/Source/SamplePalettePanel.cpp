/*
  ==============================================================================

    SamplePalettePanel.cpp
    Created: 31 Oct 2023 3:49:50pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SamplePalettePanel.h"

SamplePalettePanel::SamplePalettePanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
linkedSampleItemPanel(inSampleItemPanel)
{
    setSize(SAMPLE_PALETTE_PANEL_WIDTH - PANEL_MARGIN, SAMPLE_PALETTE_PANEL_HEIGHT - PANEL_MARGIN);
    setPanelComponents();
}

SamplePalettePanel::~SamplePalettePanel()
{
    
}

void SamplePalettePanel::paint(Graphics& g)
{
    // Draw background
    g.setColour(COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), CORNER_SIZE_MEDIUM);
    
    // Draw Title
    g.setColour(COLOUR_ACCENT_DARK);
    g.fillRoundedRectangle(getLocalBounds().
                           removeFromTop(PANEL_TITLE_HEIGHT).
                           reduced(PANEL_MARGIN / 2.0).
                           toFloat(),
                           CORNER_SIZE_MEDIUM);
    g.setFont(FONT_MEDIUM_SMALL_BOLD);
    g.setColour(COLOUR_ACCENT_LIGHT);
    g.drawFittedText("Favorites",
                     PANEL_MARGIN / 2.0,
                     PANEL_MARGIN / 2.0,
                     getWidth() - PANEL_MARGIN,
                     PANEL_TITLE_HEIGHT - PANEL_MARGIN,
                     Justification::centred,
                     1);
    
    mSampleTable->updateContent();
}

void SamplePalettePanel::setPanelComponents()
{
    // Set sample table component
    mSampleTable = std::make_unique<BlomeTableViewPalette>(currentProcessor, linkedSampleItemPanel);
    mSampleTable->setBounds(PANEL_MARGIN / 2.0,
                            PANEL_TITLE_HEIGHT,
                            getWidth() - PANEL_MARGIN,
                            getHeight() - PANEL_TITLE_HEIGHT - PANEL_MARGIN / 2.0);
    addAndMakeVisible(*mSampleTable);
}
