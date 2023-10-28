/*
  ==============================================================================

    SampleFileFilterRuleTitle.cpp
    Created: 28 Oct 2023 5:24:27pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileFilterRuleTitle.h"

SampleFileFilterRuleTitle::~SampleFileFilterRuleTitle()
{
    
}

SampleFileFilterRuleTitle::SampleFileFilterRuleTitle()
{
    
}

bool SampleFileFilterRuleTitle::matches(SampleItem const & inSampleItem)
{
    return compare<String>(inSampleItem.getTitle(), mCompareValue, mCompareOperator);
}

void SampleFileFilterRuleTitle::setCompareValue(String const & inCompareValue)
{
    mCompareValue = inCompareValue;
}
