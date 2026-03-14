/*
 ==============================================================================
 
 SampleFileFilterRuleComment.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "SampleFileFilterRuleBase.h"

/**
 SampleFileFilterRuleBase implementation for the comment property of SampleItem.
 */
class SampleFileFilterRuleComment
:
public SampleFileFilterRuleBase
{
public:
    /**
     The constructor of the comment rule.
     
     @param inRulePropertyName the rule's property name.
     */
    SampleFileFilterRuleComment(String inRulePropertyName);
    ~SampleFileFilterRuleComment();
    bool matches(SampleItem const& inSampleItem) override;
    /**
     @returns the compare value of the rule.
     */
    String getCompareValue();
    /**
     Sets the compare value of the rule.
     
     @param inCompareValue the value to set.
     */
    void setCompareValue(String const& inCompareValue);
    bool canHaveEffect() override;
    
private:
    String mCompareValue;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleFileFilterRuleComment);
};
