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
:   public Component
{
public:
    // Constructors
    PanelBase(SaemplAudioProcessor& inProcessor);
    ~PanelBase();
    
    // Methods
    void mouseEnter(const MouseEvent& event) override;
    void mouseExit(const MouseEvent& event) override;
    void paint(Graphics& g) override;
    void resized() override;
    virtual void setPanelComponents() {};
    virtual void resizePanelComponents() {};
    
protected:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(PanelBase)
    
    // Fields
    
    // Methods
    
};
