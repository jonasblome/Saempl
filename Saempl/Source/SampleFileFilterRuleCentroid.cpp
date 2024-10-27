/*
 ==============================================================================
 
 SampleFileFilterRuleCentroid.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFileFilterRuleCentroid.h"

SampleFileFilterRuleCentroid::SampleFileFilterRuleCentroid(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareValue = -1;
    mCompareOperator = GREATER_THAN;
}

SampleFileFilterRuleCentroid::~SampleFileFilterRuleCentroid()
{
    
}

bool SampleFileFilterRuleCentroid::matches(SampleItem const & inSampleItem)
{
    double propertyValue = inSampleItem.getSpectralCentroid();
    
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

double SampleFileFilterRuleCentroid::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleCentroid::setCompareValue(double const & inCompareValue)
{
    mCompareValue = inCompareValue;
}

bool SampleFileFilterRuleCentroid::canHaveEffect()
{
    return isActive && (mCompareOperator != GREATER_THAN || mCompareValue > -1);
}
