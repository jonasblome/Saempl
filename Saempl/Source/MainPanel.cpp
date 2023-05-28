/*
  ==============================================================================

    MainPanel.cpp
    Created: 22 May 2023 6:31:37am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "MainPanel.h"

MainPanel::MainPanel()
:   PanelBase()
{
    // Create thread for current plugin instance
    // This thread pre-loads the audio files into the player
    // and checks for updates in the file tree
    mThread = std::make_unique<TimeSliceThread>("audioFilePreview");
    
    // Set panel size
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    setPanelComponents();
}

MainPanel::~MainPanel()
{
    mThread->stopThread(10);
}

void MainPanel::paint(Graphics &g)
{
    PanelBase::paint(g);
}

void MainPanel::setPanelComponents()
{
    // Add header panel
    mHeaderPanel = std::make_unique<HeaderPanel>();
    mHeaderPanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(*mHeaderPanel);
    
    // Add center panel
    mCenterPanel = std::make_unique<CenterPanel>(*mThread);
    mCenterPanel->setTopLeftPosition(0, HEADER_PANEL_HEIGHT);
    addAndMakeVisible(*mCenterPanel);
}
