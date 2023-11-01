/*
  ==============================================================================

    NoiseOverlayPanel.h
    Created: 29 May 2023 9:05:27am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "BlomeStyleDefinitions.h"

/**
 Draws a layer of transparent grainy noise over the plugin editor window.
 
 Implements:
 \ref juce::Component
 */
class NoiseOverlayPanel
:   public Component
{
public:
    NoiseOverlayPanel();
    ~NoiseOverlayPanel();
    void paint(Graphics& g) override;
    /**
     Sets the grain Image component of the overlay panel.
     */
    void setPanelComponents();
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(NoiseOverlayPanel)
    
    Image mNoiseOverlay;
};
