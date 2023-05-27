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
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    
    mThread = std::make_unique<TimeSliceThread>("audioFilePreview");
    
    mHeaderPanel = std::make_unique<HeaderPanel>();
    mHeaderPanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(*mHeaderPanel);
    
    mCenterPanel = std::make_unique<CenterPanel>(*mThread);
    mCenterPanel->setTopLeftPosition(0, HEADER_PANEL_HEIGHT);
    addAndMakeVisible(*mCenterPanel);
}

MainPanel::~MainPanel()
{
    
}
