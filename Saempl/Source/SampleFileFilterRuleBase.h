/*
  ==============================================================================

    SampleFileFilterRuleBase.h
    Created: 26 Oct 2023 10:50:12am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"
#include "BlomeHelpers.h"

class SampleFileFilterRuleBase
{
public:
    // Constructors
    SampleFileFilterRuleBase();
    virtual ~SampleFileFilterRuleBase();
    
    // Methods
    virtual bool matches(SampleItem const & inSampleItem) = 0;
    template <typename T> static bool compare(T const & inPropertyValue, T const & inCompareValue, CompareOperators inCompareOperator);
    void setCompareOperator(CompareOperators inCompareOperator);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleFileFilterRuleBase)
    
protected:
    // Fields
    CompareOperators mCompareOperator;
    
    // Methods
    
};
