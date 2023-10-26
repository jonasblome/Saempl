/*
  ==============================================================================

    SampleFileFilterRule.h
    Created: 26 Oct 2023 10:50:12am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"
#include "BlomeHelpers.h"

class SampleFileFilterRule
{
public:
    // Constructors
    SampleFileFilterRule();
    virtual ~SampleFileFilterRule();
    
    // Methods
    virtual bool ruleApplies(SampleItem const & inSampleItem) = 0;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleFileFilterRule)
    
    // Fields
    CompareOperators mCompareOperator;
    
    
    // Methods
    
};
