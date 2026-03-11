/*
 ==============================================================================
 
 SampleFileFilterRulePropertyLock.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFileFilterRulePropertyLock.h"

SampleFileFilterRulePropertyLock::SampleFileFilterRulePropertyLock(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareValue = "";
    mCompareOperator = CONTAINS;
}

SampleFileFilterRulePropertyLock::~SampleFileFilterRulePropertyLock()
{
    
}

bool SampleFileFilterRulePropertyLock::matches(SampleItem const & inSampleItem)
{
    bool propertyValue = inSampleItem.getPropertiesAreLocked();
    
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

bool SampleFileFilterRulePropertyLock::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRulePropertyLock::setCompareValue(bool const & inCompareValue)
{
    mCompareValue = inCompareValue;
}

bool SampleFileFilterRulePropertyLock::canHaveEffect()
{
    return isActive;
}
