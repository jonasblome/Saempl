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
    setSize(SAMPLE_NAVIGATION_PANEL_WIDTH - PANEL_MARGIN, SAMPLE_NAVIGATION_PANEL_HEIGHT - PANEL_MARGIN / 2.0);
    setPanelComponents();
}

SampleTablePanel::~SampleTablePanel()
{
    
}

void SampleTablePanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Set background
    g.setColour(COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), CORNER_SIZE_MEDIUM);
    mSampleTable->updateContent();
}

void SampleTablePanel::setPanelComponents()
{
    // Set sample table component
    mSampleTable = std::make_unique<BlomeTableView>(sampleLibrary, linkedSampleItemPanel);
    mSampleTable->setBounds(PANEL_MARGIN / 2.0,
                            PANEL_MARGIN / 2.0,
                            getWidth() - PANEL_MARGIN,
                            getHeight() - PANEL_MARGIN);
    addAndMakeVisible(*mSampleTable);
    
    // Repaint panel
    repaint();
}

void SampleTablePanel::resizePanelComponents()
{
    if (mSampleTable != nullptr)
    {
        mSampleTable->setBounds(PANEL_MARGIN / 2.0,
                                PANEL_MARGIN / 2.0,
                                getWidth() - PANEL_MARGIN,
                                getHeight() - PANEL_MARGIN);
    }
}
