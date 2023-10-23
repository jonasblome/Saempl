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
    // Setting view model
    mSampleLibraryViewModel = std::make_unique<SampleLibraryViewModel>(currentProcessor.getSampleLibrary());
    
    // Add library panel
    mSampleLibraryPanel = std::make_unique<SampleLibraryPanel>(currentProcessor, *mSampleLibraryViewModel, linkedSampleItemPanel);
    mSampleLibraryPanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(*mSampleLibraryPanel);
    
    // Add sample table panel
    mSampleTablePanel = std::make_unique<SampleTablePanel>(currentProcessor, *mSampleLibraryViewModel);
    mSampleTablePanel->setTopLeftPosition(0, 0);
    addChildComponent(*mSampleTablePanel);
    
    // Repaint panel
    repaint();
}

void SampleNavigationPanel::resizePanelComponents()
{
    if (mSampleLibraryPanel != nullptr)
    {
        mSampleLibraryPanel->setBounds(0, 0, getWidth(), getHeight());
    }
    
    if (mSampleTablePanel != nullptr)
    {
        mSampleTablePanel->setBounds(0, 0, getWidth(), getHeight());
    }
}

void SampleNavigationPanel::showNavigationPanel(int inType)
{
    if (inType == 0)
    {
        mSampleTablePanel->setVisible(false);
        mSampleLibraryPanel->setVisible(true);
        mSampleLibraryPanel->repaint();
    }
    else if (inType == 1)
    {
        mSampleTablePanel->setVisible(true);
        mSampleLibraryPanel->setVisible(false);
        mSampleTablePanel->repaint();
    }
}
