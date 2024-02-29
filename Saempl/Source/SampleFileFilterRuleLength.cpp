/*
 ==============================================================================
 
 SampleFileFilterRuleLength.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFileFilterRuleLength.h"

SampleFileFilterRuleLength::SampleFileFilterRuleLength(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareValue = 0.0;
    mCompareOperator = GREATER_THAN;
}

SampleFileFilterRuleLength::~SampleFileFilterRuleLength()
{
    
}

bool SampleFileFilterRuleLength::matches(SampleItem const & inSampleItem)
{
    int propertyValue = inSampleItem.getLength();
    
    switch (mCompareOperator)
    {
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
        {
            jassertfalse;
            return false;
        }
    };
}

double SampleFileFilterRuleLength::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleLength::setCompareValue(double const & inCompareValue)
{
    mCompareValue = inCompareValue;
}

bool SampleFileFilterRuleLength::canHaveEffect()
{
    return isActive && (mCompareOperator != GREATER_THAN || mCompareValue != 0.0);
}
