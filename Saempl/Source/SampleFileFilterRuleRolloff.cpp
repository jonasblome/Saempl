/*
 ==============================================================================
 
 SampleFileFilterRuleRolloff.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFileFilterRuleRolloff.h"

SampleFileFilterRuleRolloff::SampleFileFilterRuleRolloff(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareValue = -1;
    mCompareOperator = GREATER_THAN;
}

SampleFileFilterRuleRolloff::~SampleFileFilterRuleRolloff()
{
    
}

bool SampleFileFilterRuleRolloff::matches(SampleItem const & inSampleItem)
{
    double propertyValue = inSampleItem.getSpectralRolloff();
    
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

double SampleFileFilterRuleRolloff::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleRolloff::setCompareValue(double const & inCompareValue)
{
    mCompareValue = inCompareValue;
}

bool SampleFileFilterRuleRolloff::canHaveEffect()
{
    return isActive && (mCompareOperator != GREATER_THAN || mCompareValue > -1);
}
