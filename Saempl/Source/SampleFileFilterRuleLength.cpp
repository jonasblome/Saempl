/*
  ==============================================================================

    SampleFileFilterRuleLength.cpp
    Created: 28 Oct 2023 5:24:42pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileFilterRuleLength.h"

SampleFileFilterRuleLength::~SampleFileFilterRuleLength()
{
    
}

SampleFileFilterRuleLength::SampleFileFilterRuleLength()
{
    
}

bool SampleFileFilterRuleLength::matches(SampleItem const & inSampleItem)
{
    return compare<double>(inSampleItem.getLength(), mCompareValue, mCompareOperator);
}

void SampleFileFilterRuleLength::setCompareValue(double const & inCompareValue)
{
    mCompareValue = inCompareValue;
}
