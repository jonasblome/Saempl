/*
 ==============================================================================
 
 PanelBase.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "BlomeStyling.h"

/**
 The base class for UI panels.
 
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
     Initialises the panel's components and adds them to the panel.
     */
    virtual void setPanelComponents() {};
    /**
     Defines how the panel's components are resized when the panel is resized.
     */
    virtual void resizePanelComponents() {};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanelBase);
};
