/*
  ==============================================================================

    SampleFileFilterRuleLoudness.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileFilterRuleLoudness.h"

SampleFileFilterRuleLoudness::SampleFileFilterRuleLoudness(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareValue = 0.0;
    mCompareOperator = GREATER_THAN;
}

SampleFileFilterRuleLoudness::~SampleFileFilterRuleLoudness()
{
    
}

bool SampleFileFilterRuleLoudness::matches(SampleItem const & inSampleItem)
{
    int propertyValue = inSampleItem.getLoudness();
    
    switch (mCompareOperator) {
        case LESS_THAN:
        {
            return propertyValue < mCompareValue;
            break;
        }
        case EQUAL_TO:
        {
            return propertyValue == mCompareValue;
            break;
        }
        case GREATER_THAN:
        {
            return propertyValue > mCompareValue;
            break;
        }
        case CONTAINS:
        {
            return false;
            break;
        }
        default:
            jassertfalse;
            return false;
    };
}

int SampleFileFilterRuleLoudness::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleLoudness::setCompareValue(int const & inCompareValue)
{
    mCompareValue = inCompareValue;
}
