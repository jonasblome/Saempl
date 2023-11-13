/*
 ==============================================================================
 
 SampleNavigationPanel.cpp
 Created: 24 Jun 2023 4:28:16pm
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleNavigationPanel.h"

SampleNavigationPanel::SampleNavigationPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:
PanelBase(inProcessor),
linkedSampleItemPanel(inSampleItemPanel)
{
    setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH, style->SAMPLE_NAVIGATION_PANEL_HEIGHT);
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
    mSampleLibraryPanel = std::make_unique<SampleLibraryPanel>(currentProcessor, linkedSampleItemPanel);
    mSampleLibraryPanel->setTopLeftPosition(0, 0);
    addChildComponent(*mSampleLibraryPanel);
    
    // Add sample table panel
    mSampleTablePanel = std::make_unique<SampleTablePanel>(currentProcessor, linkedSampleItemPanel);
    mSampleTablePanel->setTopLeftPosition(0, 0);
    addChildComponent(*mSampleTablePanel);
    
    // Set active panel
    setActiveNavigationPanel(currentProcessor.getActiveNavigationPanel());
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

void SampleNavigationPanel::setActiveNavigationPanel(NavigationPanelType& inPanelType)
{
    currentProcessor.setActiveNavigationPanel(inPanelType);
    
    switch(currentProcessor.getActiveNavigationPanel())
    {
        case PANELS_LIBRARY_PANEL:
        {
            mSampleTablePanel->setVisible(false);
            mSampleLibraryPanel->setVisible(true);
            break;
        }
        case PANELS_TABLE_PANEL:
        {
            mSampleTablePanel->setVisible(true);
            mSampleLibraryPanel->setVisible(false);
            break;
        }
        default:
            jassertfalse;
            break;
    }
}

void SampleNavigationPanel::selectRandomSample()
{
    mSampleTablePanel->selectRandomSample();
}
