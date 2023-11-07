/*
  ==============================================================================

    SampleTablePanel.h
    Created: 25 Jun 2023 5:51:12pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleItemPanel.h"
#include "BlomeTableViewNavigation.h"

/**
 Holds the table for displaying all \ref SampleLibrary::mAllSampleItems
 
 Implements:
 \ref PanelBase
 */
class SampleTablePanel
:   public PanelBase
{
public:
    /**
     The constructor for the sample table panel.
     
     @param inSampleLibrary the library object of the current plugin instance.
     @param inSampleItemPanel the panel for previewing \ref SampleItem objects.
     */
    SampleTablePanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~SampleTablePanel();
    /**
     Selects a random sample in the table view.
     */
    void selectRandomSample();
    
private:
    SampleLibrary& sampleLibrary;
    SampleItemPanel& linkedSampleItemPanel;
    std::unique_ptr<BlomeTableViewNavigation> mSampleTable;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
};
