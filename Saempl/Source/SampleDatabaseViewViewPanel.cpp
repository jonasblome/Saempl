/*
  ==============================================================================

    SampleDatabaseViewViewPanel.cpp
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleDatabaseViewViewPanel.h"

SampleDatabaseViewViewPanel::SampleDatabaseViewViewPanel(SaemplAudioProcessor* inProcessor)
:   PanelBase(inProcessor)
{
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
}

SampleDatabaseViewViewPanel::~SampleDatabaseViewViewPanel()
{
    
}
