/*
  ==============================================================================

    SampleFileFilterRuleLoudness.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleFileFilterRuleBase.h"

/**
 \ref SampleFileFilterRuleBase implementation for the loudness property of \ref SampleItem.
 */
class SampleFileFilterRuleLoudness
:
public SampleFileFilterRuleBase
{
public:
    /**
     The constructor of the loudness rule.
     
     @param inRulePropertyName the rule's property name.
     */
    SampleFileFilterRuleLoudness(String inRulePropertyName);
    ~SampleFileFilterRuleLoudness();
    bool matches(SampleItem const & inSampleItem) override;
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
    double mCompareValue;
};
