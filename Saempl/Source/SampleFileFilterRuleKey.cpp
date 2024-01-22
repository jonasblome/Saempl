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
    mCompareValue = "C";
    mCompareOperator = GREATER_THAN;
}

SampleFileFilterRuleKey::~SampleFileFilterRuleKey()
{
    
}

bool SampleFileFilterRuleKey::matches(SampleItem const & inSampleItem)
{
    String propertyValue = inSampleItem.getKey();
    
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

String SampleFileFilterRuleKey::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleKey::setCompareValue(String const & inCompareValue)
{
    mCompareValue = inCompareValue;
}
