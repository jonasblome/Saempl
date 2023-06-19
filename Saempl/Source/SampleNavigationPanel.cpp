/*
  ==============================================================================

    SampleItemNavigationPanel.cpp
    Created: 18 Jun 2023 2:07:02pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleNavigationPanel.h"

SampleNavigationPanel::SampleNavigationPanel(SaemplAudioProcessor &inProcessor)
:   PanelBase(inProcessor),
    currentProcessor(inProcessor)
{
    setSize(SAMPLE_ITEM_NAVIGATION_PANEL_WIDTH, SAMPLE_ITEM_NAVIGATION_PANEL_HEIGHT - PANEL_MARGIN * 1.5);
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
    // Add sample item filter panel and make hide it
    mSampleItemFilterPanel = std::make_unique<SampleFilterPanel>(currentProcessor);
    mSampleItemFilterPanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(*mSampleItemFilterPanel);
    mSampleItemFilterPanel->setVisible(false);
    
    // Add sample item table panel
    mSampleItemTablePanel = std::make_unique<SampleTablePanel>(currentProcessor);
    mSampleItemTablePanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(*mSampleItemTablePanel);
    
    // Repaint panel
    repaint();
}

void SampleNavigationPanel::resizePanelComponents()
{
    if (mSampleItemFilterPanel != nullptr) {
        if(mSampleItemFilterPanel->isVisible())
        {
            mSampleItemTablePanel->setBounds(0,
                                             mSampleItemFilterPanel->getHeight() + PANEL_MARGIN / 2.0,
                                             getWidth(),
                                             getHeight() - mSampleItemFilterPanel->getHeight() - PANEL_MARGIN / 2.0);
        }
        else
        {
            mSampleItemTablePanel->setBounds(0,
                                             0,
                                             getWidth(),
                                             getHeight());
        }
    }
}

void SampleNavigationPanel::toggleSampleItemFilterPanel()
{
    mSampleItemFilterPanel->setVisible(!mSampleItemFilterPanel->isVisible());
    
    if(mSampleItemFilterPanel->isVisible())
    {
        mSampleItemTablePanel->setBounds(0,
                                         mSampleItemFilterPanel->getHeight() + PANEL_MARGIN / 2.0,
                                         getWidth(),
                                         getHeight() - mSampleItemFilterPanel->getHeight() - PANEL_MARGIN / 2.0);
    }
    else
    {
        mSampleItemTablePanel->setBounds(0,
                                         0,
                                         getWidth(),
                                         getHeight());
    }
    
    mSampleItemTablePanel->repaint();
}
