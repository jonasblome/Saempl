/*
  ==============================================================================

    SampleFileFilterRuleLoudnessDecibel.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleFileFilterRuleBase.h"

/**
 SampleFileFilterRuleBase implementation for the decibel loudness property of SampleItem.
 */
class SampleFileFilterRuleLoudnessDecibel
:
public SampleFileFilterRuleBase
{
public:
    /**
     The constructor of the decibel loudness rule.
     
     @param inRulePropertyName the rule's property name.
     */
    SampleFileFilterRuleLoudnessDecibel(String inRulePropertyName);
    ~SampleFileFilterRuleLoudnessDecibel();
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
