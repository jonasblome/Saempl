/*
  ==============================================================================

    SampleDatabaseViewerPanel.h
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleDatabaseViewViewModel.h"

class SampleDatabaseViewerPanel
:   public PanelBase
{
public:
    SampleDatabaseViewerPanel(SaemplAudioProcessor* inProcessor);
    ~SampleDatabaseViewerPanel();
    
private:
    std::unique_ptr<SampleDatabaseViewViewModel> sampleDatabase;
};
