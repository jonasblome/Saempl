/*
 ==============================================================================
 
 SampleFileFilterRulePropertyLock.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "SampleFileFilterRuleBase.h"

/**
 SampleFileFilterRuleBase implementation for the PropertyLock property of SampleItem.
 */
class SampleFileFilterRulePropertyLock
:
public SampleFileFilterRuleBase
{
public:
    /**
     The constructor of the PropertyLock rule.
     
     @param inRulePropertyName the rule's property name.
     */
    SampleFileFilterRulePropertyLock(String inRulePropertyName);
    ~SampleFileFilterRulePropertyLock();
    bool matches(SampleItem const& inSampleItem) override;
    /**
     @returns the compare value of the rule.
     */
    bool getCompareValue();
    /**
     Sets the compare value of the rule.
     
     @param inCompareValue the value to set.
     */
    void setCompareValue(bool const& inCompareValue);
    bool canHaveEffect() override;
    
private:
    bool mCompareValue = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleFileFilterRulePropertyLock);
};
