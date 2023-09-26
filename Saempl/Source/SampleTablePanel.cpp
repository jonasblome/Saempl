/*
  ==============================================================================

    SampleTablePanel.cpp
    Created: 25 Jun 2023 5:51:12pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleTablePanel.h"

SampleTablePanel::SampleTablePanel(SaemplAudioProcessor& inProcessor, SampleLibraryViewModel& inSampleLibraryViewModel)
:   PanelBase(inProcessor),
    currentProcessor(inProcessor),
    sampleLibraryViewModel(inSampleLibraryViewModel)
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
    g.fillRoundedRectangle(getLocalBounds().toFloat(), MEDIUM_CORNER_SIZE);
    mSampleItemTable->refresh();
}

void SampleTablePanel::setPanelComponents()
{
    // Set sample table component
    mSampleItemTable = std::make_unique<BlomeTableView>(sampleLibraryViewModel);
    mSampleItemTable->setBounds(PANEL_MARGIN / 2.0,
                                PANEL_MARGIN / 2.0,
                                getWidth() - PANEL_MARGIN,
                                getHeight() - PANEL_MARGIN);
    addAndMakeVisible(*mSampleItemTable);
    
    // Repaint panel
    repaint();
}

void SampleTablePanel::resizePanelComponents()
{
    if (mSampleItemTable != nullptr)
    {
        mSampleItemTable->setBounds(PANEL_MARGIN / 2.0, PANEL_MARGIN / 2.0, getWidth() - PANEL_MARGIN, getHeight() - PANEL_MARGIN);
    }
}
