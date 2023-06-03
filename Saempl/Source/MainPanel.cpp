/*
  ==============================================================================

    MainPanel.cpp
    Created: 22 May 2023 6:31:37am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "MainPanel.h"

MainPanel::MainPanel(SaemplAudioProcessor& inProcessor)
:   PanelBase(inProcessor),
    currentProcessor(inProcessor)
{
    // Set panel size
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    setPanelComponents();
}

MainPanel::~MainPanel()
{
    
}

void MainPanel::paint(Graphics &g)
{
    PanelBase::paint(g);
}

void MainPanel::setPanelComponents()
{
    // Add header panel
    mHeaderPanel = std::make_unique<HeaderPanel>(currentProcessor);
    mHeaderPanel->setTopLeftPosition(Blome_PanelMargin / 2.0, Blome_PanelMargin / 2.0);
    addAndMakeVisible(*mHeaderPanel);
    
    // Add center panel
    mCenterPanel = std::make_unique<CenterPanel>(currentProcessor);
    mCenterPanel->setTopLeftPosition(Blome_PanelMargin / 2.0, HEADER_PANEL_HEIGHT);
    addAndMakeVisible(*mCenterPanel);
}
