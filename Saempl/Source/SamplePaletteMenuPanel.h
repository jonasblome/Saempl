/*
  ==============================================================================

    SampleItemPaletteMenuPanel.h
    Created: 18 Jun 2023 6:29:52pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SamplePalettePanel.h"

class SamplePaletteMenuPanel
:   public PanelBase
{
public:
    // Constructors
    SamplePaletteMenuPanel(SaemplAudioProcessor& inProcessor, SamplePalettePanel& inSampleItemPalettePanel);
    ~SamplePaletteMenuPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
private:
    // Fields
    SamplePalettePanel& linkedSampleItemPalettePanel;
    
    // Methods
    
};
