/*
 ==============================================================================
 
 SampleFileFilterRuleDynamicRange.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFileFilterRuleDynamicRange.h"

SampleFileFilterRuleDynamicRange::SampleFileFilterRuleDynamicRange(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareValue = -1;
    mCompareOperator = GREATER_THAN;
}

SampleFileFilterRuleDynamicRange::~SampleFileFilterRuleDynamicRange()
{
    
}

bool SampleFileFilterRuleDynamicRange::matches(SampleItem const & inSampleItem)
{
    double propertyValue = inSampleItem.getDynamicRange();
    
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

double SampleFileFilterRuleDynamicRange::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleDynamicRange::setCompareValue(double const & inCompareValue)
{
    mCompareValue = inCompareValue;
}

bool SampleFileFilterRuleDynamicRange::canHaveEffect()
{
    return isActive && (mCompareOperator != GREATER_THAN || mCompareValue > -1);
}
