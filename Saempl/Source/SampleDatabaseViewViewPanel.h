/*
  ==============================================================================

    SampleDatabaseViewViewPanel.h
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleDatabaseViewViewModel.h"

class SampleDatabaseViewViewPanel
:   public PanelBase
{
public:
    SampleDatabaseViewViewPanel(SaemplAudioProcessor* inProcessor);
    ~SampleDatabaseViewViewPanel();
    
private:
    std::unique_ptr<SampleDatabaseViewViewModel> sampleDatabase;
};
