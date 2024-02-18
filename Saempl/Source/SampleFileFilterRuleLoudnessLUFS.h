/*
 ==============================================================================
 
 SampleFileFilterRuleLoudnessLUFS.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "SampleFileFilterRuleBase.h"

/**
 SampleFileFilterRuleBase implementation for the LUFS loudness property of SampleItem.
 */
class SampleFileFilterRuleLoudnessLUFS
:
public SampleFileFilterRuleBase
{
public:
    /**
     The constructor of the LUFS loudness rule.
     
     @param inRulePropertyName the rule's property name.
     */
    SampleFileFilterRuleLoudnessLUFS(String inRulePropertyName);
    ~SampleFileFilterRuleLoudnessLUFS();
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
};
