/*
  ==============================================================================

    NoiseOverlayPanel.h
    Created: 29 May 2023 9:05:27am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "PanelBase.h"

class NoiseOverlayPanel
:   public Component
{
public:
    // Constructors
    NoiseOverlayPanel();
    ~NoiseOverlayPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents();
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(NoiseOverlayPanel)
    
    // Fields
    Image mNoiseOverlay;
    
    // Methods
    
};
