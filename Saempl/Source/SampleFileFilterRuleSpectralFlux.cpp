/*
 ==============================================================================
 
 SampleFileFilterRuleSpectralFlux.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFileFilterRuleSpectralFlux.h"

SampleFileFilterRuleSpectralFlux::SampleFileFilterRuleSpectralFlux(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareValue = -1;
    mCompareOperator = GREATER_THAN;
}

SampleFileFilterRuleSpectralFlux::~SampleFileFilterRuleSpectralFlux()
{
    
}

bool SampleFileFilterRuleSpectralFlux::matches(SampleItem const & inSampleItem)
{
    double propertyValue = inSampleItem.getSpectralFlux();
    
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

double SampleFileFilterRuleSpectralFlux::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleSpectralFlux::setCompareValue(double const & inCompareValue)
{
    mCompareValue = inCompareValue;
}

bool SampleFileFilterRuleSpectralFlux::canHaveEffect()
{
    return isActive && (mCompareOperator != GREATER_THAN || mCompareValue > -1);
}
