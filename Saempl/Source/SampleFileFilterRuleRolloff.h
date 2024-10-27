/*
 ==============================================================================
 
 SampleFileFilterRuleRolloff.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "SampleFileFilterRuleBase.h"

/**
 SampleFileFilterRuleBase implementation for the Rolloff property of SampleItem.
 */
class SampleFileFilterRuleRolloff
:
public SampleFileFilterRuleBase
{
public:
    /**
     The constructor of the Rolloff rule.
     
     @param inRulePropertyName the rule's property name.
     */
    SampleFileFilterRuleRolloff(String inRulePropertyName);
    ~SampleFileFilterRuleRolloff();
    bool matches(SampleItem const & inSampleItem) override;
    /**
     @returns the compare value of the rule.
     */
    double getCompareValue();
    /**
     Sets the compare value of the rule.
     
     @param inCompareValue the value to set.
     */
    void setCompareValue(double const & inCompareValue);
    bool canHaveEffect() override;
    
private:
    double mCompareValue;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleFileFilterRuleRolloff);
};
