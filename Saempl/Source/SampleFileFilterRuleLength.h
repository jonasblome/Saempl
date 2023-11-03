/*
  ==============================================================================

    SampleFileFilterRuleLength.h
    Created: 28 Oct 2023 5:24:42pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleFileFilterRuleBase.h"

/**
 \ref SampleFileFilterRuleBase implementation for the length property of \ref SampleItem.
 */
class SampleFileFilterRuleLength
:   public SampleFileFilterRuleBase
{
public:
    /**
     The constructor of the length rule.
     
     @param inRulePropertyName the rule's property name.
     */
    SampleFileFilterRuleLength(String inRulePropertyName);
    ~SampleFileFilterRuleLength();bool matches(SampleItem const & inSampleItem) override;
    /**
     @returns the compare value of the rule.
     */
    int getCompareValue();
    /**
     Sets the compare value of the rule.
     
     @param inCompareValue the value to set.
     */
    void setCompareValue(int const & inCompareValue);
    
private:
    // JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleFileFilterRuleLength)
    
    double mCompareValue;
};
