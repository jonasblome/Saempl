/*
 ==============================================================================
 
 SampleFileFilterRuleChromaFlux.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "SampleFileFilterRuleBase.h"

/**
 SampleFileFilterRuleBase implementation for the ChromaFlux property of SampleItem.
 */
class SampleFileFilterRuleChromaFlux
:
public SampleFileFilterRuleBase
{
public:
    /**
     The constructor of the ChromaFlux rule.
     
     @param inRulePropertyName the rule's property name.
     */
    SampleFileFilterRuleChromaFlux(String inRulePropertyName);
    ~SampleFileFilterRuleChromaFlux();
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleFileFilterRuleChromaFlux);
};
