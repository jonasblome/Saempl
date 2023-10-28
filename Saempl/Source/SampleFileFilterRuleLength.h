/*
  ==============================================================================

    SampleFileFilterRuleLength.h
    Created: 28 Oct 2023 5:24:42pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleFileFilterRuleBase.h"

class SampleFileFilterRuleLength
:   public SampleFileFilterRuleBase
{
public:
    // Constructors
    SampleFileFilterRuleLength();
    ~SampleFileFilterRuleLength();
    
    // Methods
    bool matches(SampleItem const & inSampleItem) override;
    void setCompareValue(double const & inCompareValue);
    
private:
    // Fields
    double mCompareValue;
    
    // Methods
    
};
