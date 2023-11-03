/*
  ==============================================================================

    SampleFileFilterRuleTitle.h
    Created: 28 Oct 2023 5:24:27pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleFileFilterRuleBase.h"

/**
 \ref SampleFileFilterRuleBase implementation for the title property of \ref SampleItem.
 */
class SampleFileFilterRuleTitle
:   public SampleFileFilterRuleBase
{
public:
    /**
     The constructor of the title rule.
     
     @param inRulePropertyName the rule's property name.
     */
    SampleFileFilterRuleTitle(String inRulePropertyName);
    ~SampleFileFilterRuleTitle();
    bool matches(SampleItem const & inSampleItem) override;
    /**
     @returns the compare value of the rule.
     */
    String getCompareValue();
    /**
     Sets the compare value of the rule.
     
     @param inCompareValue the value to set.
     */
    void setCompareValue(String const & inCompareValue);
    
private:
    // JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleFileFilterRuleTitle)
    
    String mCompareValue;
};
