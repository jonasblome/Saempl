/*
  ==============================================================================

    PanelBase.h
    Created: 22 May 2023 6:31:30am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "BlomeStyleDefinitions.h"

class PanelBase
:   public juce::Component
{
public:
    PanelBase(SaemplAudioProcessor* inProcessor);
    ~PanelBase();
    
    void mouseEnter(const MouseEvent& event) override;
    
    void mouseExit(const MouseEvent& event) override;
    
    void paint(Graphics& g) override;
    
protected:
    SaemplAudioProcessor* mProcessor;
};
