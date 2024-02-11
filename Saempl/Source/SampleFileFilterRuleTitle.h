/*
 ==============================================================================
 
 SampleFileFilterRuleTitle.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "SampleFileFilterRuleBase.h"

/**
 SampleFileFilterRuleBase implementation for the title property of SampleItem.
 */
class SampleFileFilterRuleTitle
:
public SampleFileFilterRuleBase
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
    bool canHaveEffect() override;
    
private:
    String mCompareValue;
};
