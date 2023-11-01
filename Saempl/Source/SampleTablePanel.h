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

class SampleTablePanel
:   public PanelBase
{
public:
    // Constructors
    SampleTablePanel(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel);
    ~SampleTablePanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleTablePanel)
    
    // Fields
    SampleLibrary& sampleLibrary;
    SampleItemPanel& linkedSampleItemPanel;
    std::unique_ptr<BlomeTableViewNavigation> mSampleTable;
    
    // Methods
    
};
