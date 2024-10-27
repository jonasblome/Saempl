/*
 ==============================================================================
 
 SampleFileFilterRuleZeroCrossingRate.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "SampleFileFilterRuleBase.h"

/**
 SampleFileFilterRuleBase implementation for the ZeroCrossingRate property of SampleItem.
 */
class SampleFileFilterRuleZeroCrossingRate
:
public SampleFileFilterRuleBase
{
public:
    /**
     The constructor of the ZeroCrossingRate rule.
     
     @param inRulePropertyName the rule's property name.
     */
    SampleFileFilterRuleZeroCrossingRate(String inRulePropertyName);
    ~SampleFileFilterRuleZeroCrossingRate();
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleFileFilterRuleZeroCrossingRate);
};
