/*
  ==============================================================================

    SampleItemNavigationMenuPanel.h
    Created: 18 Jun 2023 6:01:12pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleNavigationPanel.h"

class SampleNavigationMenuPanel
:   public PanelBase
{
public:
    // Constructors
    SampleNavigationMenuPanel(SaemplAudioProcessor& inProcessor, SampleNavigationPanel& inSampleItemNavigationPanel);
    ~SampleNavigationMenuPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
private:
    // Fields
    SampleNavigationPanel& linkedSampleItemNavigationPanel;
    std::unique_ptr<ToggleButton> mToggleSampleItemFilterPanelButton;
    
    // Methods
    
};
