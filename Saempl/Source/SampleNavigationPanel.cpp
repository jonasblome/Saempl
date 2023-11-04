/*
  ==============================================================================

    SampleNavigationPanel.cpp
    Created: 24 Jun 2023 4:28:16pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleNavigationPanel.h"

SampleNavigationPanel::SampleNavigationPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:   PanelBase(),
    linkedSampleItemPanel(inSampleItemPanel),
    sampleLibrary(inProcessor.getSampleLibrary())
{
    setSize(SAMPLE_NAVIGATION_PANEL_WIDTH, SAMPLE_NAVIGATION_PANEL_HEIGHT);
    setPanelComponents();
}

SampleNavigationPanel::~SampleNavigationPanel()
{
    
}

void SampleNavigationPanel::paint(Graphics& g)
{
    
}

void SampleNavigationPanel::setPanelComponents()
{
    // Add library panel
    mSampleLibraryPanel = std::make_unique<SampleLibraryPanel>(sampleLibrary, linkedSampleItemPanel);
    mSampleLibraryPanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(*mSampleLibraryPanel);
    
    // Add sample table panel
    mSampleTablePanel = std::make_unique<SampleTablePanel>(sampleLibrary, linkedSampleItemPanel);
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

void SampleNavigationPanel::showNavigationPanel(NavigationPanelType inPanelType)
{
    mActiveNavigationPanelType = inPanelType;
    
    switch(mActiveNavigationPanelType)
    {
        case PANELS_LIBRARY_PANEL:
        {
            mSampleTablePanel->setVisible(false);
            mSampleLibraryPanel->setVisible(true);
            mSampleLibraryPanel->repaint();
            break;
        }
        case PANELS_TABLE_PANEL:
        {
            mSampleTablePanel->setVisible(true);
            mSampleLibraryPanel->setVisible(false);
            mSampleTablePanel->repaint();
            break;
        }
        default:
            jassertfalse;
            break;
    }
}

NavigationPanelType SampleNavigationPanel::getActiveNavigationPanelType()
{
    return mActiveNavigationPanelType;
}
