/*
 ==============================================================================
 
 SampleFileFilterRuleChromaFlux.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFileFilterRuleChromaFlux.h"

SampleFileFilterRuleChromaFlux::SampleFileFilterRuleChromaFlux(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareValue = -1;
    mCompareOperator = GREATER_THAN;
}

SampleFileFilterRuleChromaFlux::~SampleFileFilterRuleChromaFlux()
{
    
}

bool SampleFileFilterRuleChromaFlux::matches(SampleItem const & inSampleItem)
{
    double propertyValue = inSampleItem.getChromaFlux();
    
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

double SampleFileFilterRuleChromaFlux::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleChromaFlux::setCompareValue(double const & inCompareValue)
{
    mCompareValue = inCompareValue;
}

bool SampleFileFilterRuleChromaFlux::canHaveEffect()
{
    return isActive && (mCompareOperator != GREATER_THAN || mCompareValue > -1);
}
