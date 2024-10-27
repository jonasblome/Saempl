/*
 ==============================================================================
 
 SampleFileFilterRuleZeroCrossingRate.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFileFilterRuleZeroCrossingRate.h"

SampleFileFilterRuleZeroCrossingRate::SampleFileFilterRuleZeroCrossingRate(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareValue = -1;
    mCompareOperator = GREATER_THAN;
}

SampleFileFilterRuleZeroCrossingRate::~SampleFileFilterRuleZeroCrossingRate()
{
    
}

bool SampleFileFilterRuleZeroCrossingRate::matches(SampleItem const & inSampleItem)
{
    double propertyValue = inSampleItem.getZeroCrossingRate();
    
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

double SampleFileFilterRuleZeroCrossingRate::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleZeroCrossingRate::setCompareValue(double const & inCompareValue)
{
    mCompareValue = inCompareValue;
}

bool SampleFileFilterRuleZeroCrossingRate::canHaveEffect()
{
    return isActive && (mCompareOperator != GREATER_THAN || mCompareValue > -1);
}
