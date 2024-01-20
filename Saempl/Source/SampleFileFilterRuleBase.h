/*
 ==============================================================================
 
 SampleFileFilterRuleBase.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"
#include "BlomeHelpers.h"

/**
 The base class for SampleFileFilter rules.
 
 Can be toggled and holds a compare operator.
 */
class SampleFileFilterRuleBase
{
public:
    /**
     The constructor of the rule base.
     
     @param inRulePropertyName the rule's property name.
     */
    SampleFileFilterRuleBase(String inRulePropertyName);
    virtual ~SampleFileFilterRuleBase();
    /**
     Determines whether the given sample item matches the rule.
     
     @param inSampleItem the sample item to check
     
     @returns whether the item matches the rule.
     */
    virtual bool matches(SampleItem const & inSampleItem) = 0;
    /**
     @returns the compare operator of the rule.
     */
    CompareOperators getCompareOperator();
    /**
     Sets the compare operator of the current rule.
     
     @param inCompareOperator the compare operator to set.
     */
    void setCompareOperator(CompareOperators inCompareOperator);
    /**
     @returns the rule's property name.
     */
    String getRulePropertyName();
    /**
     @returns whether the rule is active.
     */
    bool getIsActive();
    /**
     Sets if the rule is active.
     
     @param inIsActive whether the rule should be active.
     */
    void setIsActive(bool inIsActive);
    
protected:
    CompareOperators mCompareOperator;
    String mRulePropertyName;
    bool isActive;
};
