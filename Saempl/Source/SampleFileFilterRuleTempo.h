/*
  ==============================================================================

    SampleFileFilterRuleTempo.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleFileFilterRuleBase.h"

/**
 SampleFileFilterRuleBase implementation for the tempo property of SampleItem.
 */
class SampleFileFilterRuleTempo
:
public SampleFileFilterRuleBase
{
public:
    /**
     The constructor of the tempo rule.
     
     @param inRulePropertyName the rule's property name.
     */
    SampleFileFilterRuleTempo(String inRulePropertyName);
    ~SampleFileFilterRuleTempo();
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
    bool canHaveEffect() override;
    
private:
    int mCompareValue;
};
