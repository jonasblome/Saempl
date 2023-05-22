/*
  ==============================================================================

    MainPanel.h
    Created: 22 May 2023 6:31:37am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleDatabaseViewerPanel.h"

class MainPanel
:   public PanelBase
{
public:
    MainPanel(SaemplAudioProcessor* inProcessor);
    ~MainPanel();
    
private:
    std::unique_ptr<SampleDatabaseViewerPanel> sampleDatabaseViewer;
    
};
