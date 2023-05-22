/*
  ==============================================================================

    MainPanel.cpp
    Created: 22 May 2023 6:31:37am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "MainPanel.h"

MainPanel::MainPanel(SaemplAudioProcessor* inProcessor)
:   PanelBase(inProcessor)
{
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    
    sampleDatabaseViewer = std::make_unique<SampleDatabaseViewerPanel>(inProcessor);
}

MainPanel::~MainPanel()
{
    
}
