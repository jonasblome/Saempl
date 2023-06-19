/*
  ==============================================================================

    SampleItemTablePanel.h
    Created: 18 Jun 2023 2:10:18pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"

class SampleTablePanel
:   public PanelBase
{
public:
    // Constructors
    SampleTablePanel(SaemplAudioProcessor& inProcessor);
    ~SampleTablePanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
private:
    // Fields
    
    // Methods
    
};
