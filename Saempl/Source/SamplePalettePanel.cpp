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
    setSize(style->SAMPLE_PALETTE_PANEL_WIDTH - style->PANEL_MARGIN, style->SAMPLE_PALETTE_PANEL_HEIGHT - style->PANEL_MARGIN);
    setPanelComponents();
}

SamplePalettePanel::~SamplePalettePanel()
{
    
}

void SamplePalettePanel::paint(Graphics& g)
{
    // Draw background
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), style->CORNER_SIZE_MEDIUM);
    
    // Draw Title
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.fillRoundedRectangle(getLocalBounds().
                           removeFromTop(style->PANEL_TITLE_HEIGHT).
                           reduced(style->PANEL_MARGIN / 2.0).
                           toFloat(),
                           style->CORNER_SIZE_MEDIUM);
    g.setFont(style->FONT_MEDIUM_SMALL_BOLD);
    g.setColour(style->COLOUR_ACCENT_LIGHT);
    g.drawFittedText("Favorites",
                     style->PANEL_MARGIN / 2.0,
                     style->PANEL_MARGIN / 2.0,
                     getWidth() - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN,
                     Justification::centred,
                     1);
    
    mSampleTable->updateContent();
}

void SamplePalettePanel::setPanelComponents()
{
    // Set sample table component
    mSampleTable = std::make_unique<BlomeTableViewPalette>(currentProcessor, linkedSampleItemPanel);
    mSampleTable->setBounds(style->PANEL_MARGIN / 2.0,
                            style->PANEL_TITLE_HEIGHT,
                            getWidth() - style->PANEL_MARGIN,
                            getHeight() - style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN / 2.0);
    addAndMakeVisible(*mSampleTable);
}
