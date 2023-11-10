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

/**
 The base class for UI panels.
 
 Implements:
 \ref juce::Component
 Handles setting, painting and resizing it's children and components.
 */
class PanelBase
:
public Component
{
public:
    /**
     The constructor for the panel base class.
     
     @param inProcessor the processor reference of the current plugin instance.
     */
    PanelBase(SaemplAudioProcessor& inProcessor);
    ~PanelBase();
    
protected:
    SaemplAudioProcessor& currentProcessor;
    
    void mouseEnter(MouseEvent const & event) override;
    void mouseExit(MouseEvent const & event) override;
    void paint(Graphics& g) override;
    void resized() override;
    /**
     Initializes the panel's components and adds them to the panel.
     */
    virtual void setPanelComponents() {};
    /**
     Defines how the panel's components are resized when the panel is resized.
     */
    virtual void resizePanelComponents() {};
};
