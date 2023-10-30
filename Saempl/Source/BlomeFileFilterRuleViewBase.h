/*
  ==============================================================================

    BlomeFileFilterRuleViewBase.h
    Created: 29 Oct 2023 12:14:30pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "BlomeStyleDefinitions.h"
#include "SampleLibrary.h"

class BlomeFileFilterRuleViewBase
:   public Component
{
public:
    // Constructors
    BlomeFileFilterRuleViewBase(SampleLibrary& inSampleLibrary);
    virtual ~BlomeFileFilterRuleViewBase();
    
    // Methods
    void paint(Graphics& g) override;
    virtual void setPanelComponents() = 0;
    virtual void resized() override = 0;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(BlomeFileFilterRuleViewBase)
    
protected:
    // Fields
    SampleLibrary& linkedSampleLibrary;
    
    // Methods
    
    
};
