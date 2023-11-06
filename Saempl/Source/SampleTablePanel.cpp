/*
  ==============================================================================

    SampleTablePanel.cpp
    Created: 25 Jun 2023 5:51:12pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleTablePanel.h"

SampleTablePanel::SampleTablePanel(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel)
:   PanelBase(),
    sampleLibrary(inSampleLibrary),
    linkedSampleItemPanel(inSampleItemPanel)
{
    setSize(SAMPLE_NAVIGATION_PANEL_WIDTH - PANEL_MARGIN / 2.0, SAMPLE_NAVIGATION_PANEL_HEIGHT - PANEL_MARGIN / 2.0);
    setPanelComponents();
}

SampleTablePanel::~SampleTablePanel()
{
    
}

void SampleTablePanel::paint(Graphics& g)
{
    // Set background
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
    g.drawFittedText("All Samples - " + sampleLibrary.getCurrentLibraryPath(),
                     PANEL_MARGIN / 2.0,
                     PANEL_MARGIN / 2.0,
                     getWidth() - PANEL_MARGIN,
                     PANEL_TITLE_HEIGHT - PANEL_MARGIN,
                     Justification::centred,
                     1);
    
    mSampleTable->updateContent();
}

void SampleTablePanel::setPanelComponents()
{
    // Set sample table component
    mSampleTable = std::make_unique<BlomeTableViewNavigation>(sampleLibrary, linkedSampleItemPanel);
    mSampleTable->setBounds(PANEL_MARGIN / 2.0,
                            PANEL_TITLE_HEIGHT,
                            getWidth() - PANEL_MARGIN,
                            getHeight() - PANEL_TITLE_HEIGHT - PANEL_MARGIN / 2.0);
    addAndMakeVisible(*mSampleTable);
}

void SampleTablePanel::resizePanelComponents()
{
    if (mSampleTable != nullptr)
    {
        mSampleTable->setBounds(PANEL_MARGIN / 2.0,
                                PANEL_TITLE_HEIGHT,
                                getWidth() - PANEL_MARGIN,
                                getHeight() - PANEL_TITLE_HEIGHT - PANEL_MARGIN / 2.0);
    }
}
