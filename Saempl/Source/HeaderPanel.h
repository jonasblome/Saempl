/*
  ==============================================================================

    HeaderPanel.h
    Created: 27 May 2023 11:12:36am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"

class HeaderPanel
:   public PanelBase
{
public:
    // Constructors
    HeaderPanel(SaemplAudioProcessor& inProcessor);
    ~HeaderPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
private:
    // Fields
    SaemplAudioProcessor& currentProcessor;
    
    // Methods
    
};
