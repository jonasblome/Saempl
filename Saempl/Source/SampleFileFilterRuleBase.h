/*
  ==============================================================================

    SampleFileFilterRuleBase.h
    Created: 26 Oct 2023 10:50:12am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"
#include "BlomeHelpers.h"

class SampleFileFilterRuleBase
{
public:
    // Constructors
    SampleFileFilterRuleBase(String inRuleType);
    virtual ~SampleFileFilterRuleBase();
    
    // Methods
    virtual bool matches(SampleItem const & inSampleItem) = 0;
    CompareOperators getCompareOperator();
    void setCompareOperator(CompareOperators inCompareOperator);
    String getRuleType();
    bool getIsActive();
    void setIsActive(bool inIsActive);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleFileFilterRuleBase)
    
protected:
    // Fields
    CompareOperators mCompareOperator;
    String mRuleType;
    bool isActive;
    
    // Methods
    
};
