/*
  ==============================================================================

    SampleFileFilterRuleTitle.h
    Created: 28 Oct 2023 5:24:27pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleFileFilterRuleBase.h"

class SampleFileFilterRuleTitle
:   public SampleFileFilterRuleBase
{
public:
    // Constructors
    SampleFileFilterRuleTitle(String inRuleType);
    ~SampleFileFilterRuleTitle();
    
    // Methods
    bool matches(SampleItem const & inSampleItem) override;
    String getCompareValue();
    void setCompareValue(String const & inCompareValue);
    
private:
    // Fields
    String mCompareValue;
    
    // Methods
    
};
