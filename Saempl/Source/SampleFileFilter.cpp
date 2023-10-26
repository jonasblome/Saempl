/*
  ==============================================================================

    SampleFileFilter.cpp
    Created: 10 Jun 2023 6:14:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileFilter.h"

SampleFileFilter::SampleFileFilter()
{
    
}

SampleFileFilter::~SampleFileFilter()
{
    
}

bool SampleFileFilter::rulesApply(SampleItem& inSampleItem)
{
    for (SampleFileFilterRule* rule : mFilterRules)
    {
        if (!rule->ruleApplies(inSampleItem))
        {
            return false;
        }
    }
    
    return true;
}

void SampleFileFilter::addFilterRule(SampleFileFilterRule* inFilterRule)
{
    mFilterRules.add(inFilterRule);
}
