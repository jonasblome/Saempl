/*
  ==============================================================================

    SampleNavigationPanel.cpp
    Created: 24 Jun 2023 4:28:16pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleNavigationPanel.h"

SampleNavigationPanel::SampleNavigationPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:   PanelBase(inProcessor),
    currentProcessor(inProcessor),
    linkedSampleItemPanel(inSampleItemPanel)
{
    setSize(SAMPLE_NAVIGATION_PANEL_WIDTH, SAMPLE_NAVIGATION_PANEL_HEIGHT - PANEL_MARGIN / 2.0);
    setPanelComponents();
}

SampleNavigationPanel::~SampleNavigationPanel()
{
    
}

void SampleNavigationPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
}

void SampleNavigationPanel::setPanelComponents()
{
    // Add panel for database views
    mSampleLibrariesPanel = std::make_unique<SampleLibraryPanel>(currentProcessor, linkedSampleItemPanel);
    mSampleLibrariesPanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(*mSampleLibrariesPanel);
}

void SampleNavigationPanel::resizePanelComponents()
{
    if (mSampleLibrariesPanel != nullptr)
    {
        mSampleLibrariesPanel->setBounds(0, 0, getWidth(), getHeight());
    }
}
