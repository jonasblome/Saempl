/*
  ==============================================================================

    SampleFileFilter.h
    Created: 10 Jun 2023 6:14:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"
#include "BlomeHelpers.h"
#include "SampleFileFilterRule.h"

class SampleFileFilter
{
public:
    // Constructors
    SampleFileFilter();
    ~SampleFileFilter();
    
    // Methods
    bool rulesApply(SampleItem& inFile);
    void addFilterRule(SampleFileFilterRule* inRule);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleFileFilter)
    
    // Fields
    OwnedArray<SampleFileFilterRule> mFilterRules;
    
    // Methods
    
};
