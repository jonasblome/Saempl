/*
 ==============================================================================
 
 SampleFileFilterRuleTempo.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFileFilterRuleTempo.h"

SampleFileFilterRuleTempo::SampleFileFilterRuleTempo(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareValue = -1;
    mCompareOperator = GREATER_THAN;
}

SampleFileFilterRuleTempo::~SampleFileFilterRuleTempo()
{
    
}

bool SampleFileFilterRuleTempo::matches(SampleItem const & inSampleItem)
{
    int propertyValue = inSampleItem.getTempo();
    
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
            jassertfalse;
            return false;
    };
}

int SampleFileFilterRuleTempo::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleTempo::setCompareValue(int const & inCompareValue)
{
    mCompareValue = inCompareValue;
}

bool SampleFileFilterRuleTempo::canHaveEffect()
{
    return isActive && (mCompareOperator != GREATER_THAN || mCompareValue != -1);
}
