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
    mCompareValue = "No key detected";
    mCompareOperator = EQUAL_TO;
}

SampleFileFilterRuleKey::~SampleFileFilterRuleKey()
{
    
}

bool SampleFileFilterRuleKey::matches(SampleItem const & inSampleItem)
{
    String propertyValue = KEY_INDEX_TO_KEY_NAME[inSampleItem.getKey()];
    
    switch (mCompareOperator)
    {
        case LESS_THAN:
        {
            return false;
            break;
        }
        case EQUAL_TO:
        {
            return propertyValue == mCompareValue;
            break;
        }
        case GREATER_THAN:
        {
            return false;
            break;
        }
        case CONTAINS:
        {
            return false;
            break;
        }
        default:
        {
            jassertfalse;
            return false;
        }
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

bool SampleFileFilterRuleKey::canHaveEffect()
{
    return isActive;
}
