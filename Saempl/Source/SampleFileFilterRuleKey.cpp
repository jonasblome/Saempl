/*
  ==============================================================================

    SampleFileFilterRuleKey.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileFilterRuleKey.h"

SampleFileFilterRuleKey::SampleFileFilterRuleKey(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareValue = 0;
    mCompareOperator = CONTAINS;
}

SampleFileFilterRuleKey::~SampleFileFilterRuleKey()
{
    
}

bool SampleFileFilterRuleKey::matches(SampleItem const & inSampleItem)
{
    int propertyValue = inSampleItem.getKey();
    
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

int SampleFileFilterRuleKey::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleKey::setCompareValue(int const & inCompareValue)
{
    mCompareValue = inCompareValue;
}

bool SampleFileFilterRuleKey::canHaveEffect()
{
    return isActive;
}
