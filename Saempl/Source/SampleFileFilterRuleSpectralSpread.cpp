/*
 ==============================================================================
 
 SampleFileFilterRuleSpectralSpread.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFileFilterRuleSpectralSpread.h"

SampleFileFilterRuleSpectralSpread::SampleFileFilterRuleSpectralSpread(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareValue = -1;
    mCompareOperator = GREATER_THAN;
}

SampleFileFilterRuleSpectralSpread::~SampleFileFilterRuleSpectralSpread()
{
    
}

bool SampleFileFilterRuleSpectralSpread::matches(SampleItem const & inSampleItem)
{
    double propertyValue = inSampleItem.getSpectralSpread();
    
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

double SampleFileFilterRuleSpectralSpread::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleSpectralSpread::setCompareValue(double const & inCompareValue)
{
    mCompareValue = inCompareValue;
}

bool SampleFileFilterRuleSpectralSpread::canHaveEffect()
{
    return isActive && (mCompareOperator != GREATER_THAN || mCompareValue > -1);
}
