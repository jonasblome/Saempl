/*
  ==============================================================================

    SampleTablePanel.h
    Created: 25 Jun 2023 5:51:12pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleLibraryViewModel.h"
#include "SampleItemPanel.h"
#include "BlomeTableView.h"

class SampleTablePanel
:   public PanelBase
{
public:
    // Constructors
    SampleTablePanel(SaemplAudioProcessor& inProcessor, SampleLibraryViewModel& inSampleLibraryViewModel);
    ~SampleTablePanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
    
private:
    // Fields
    SaemplAudioProcessor& currentProcessor;
    SampleLibraryViewModel& sampleLibraryViewModel;
    std::unique_ptr<BlomeTableView> mSampleItemTable;
    
    // Methods
    
};
