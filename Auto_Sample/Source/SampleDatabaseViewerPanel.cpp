/*
  ==============================================================================

    SampleDatabaseViewerPanel.cpp
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleDatabaseViewerPanel.h"

SampleDatabaseViewerPanel::SampleDatabaseViewerPanel(SaemplAudioProcessor* inProcessor)
:   PanelBase(inProcessor)
{
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
}

SampleDatabaseViewerPanel::~SampleDatabaseViewerPanel()
{
    
}
