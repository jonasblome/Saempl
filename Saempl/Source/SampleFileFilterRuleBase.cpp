/*
  ==============================================================================

    SampleFileFilterRuleBase.cpp
    Created: 26 Oct 2023 10:50:12am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileFilterRuleBase.h"

SampleFileFilterRuleBase::SampleFileFilterRuleBase()
{
    
}

template <typename T> bool SampleFileFilterRuleBase::compare(const T &inPropertyValue,
                                                             const T &inCompareValue,
                                                             CompareOperators const inCompareOperator)
{
    switch (inCompareOperator) {
        case LessThan:
        {
            return inPropertyValue < inCompareValue;
            break;
        }
        case EqualTo:
        {
            return inPropertyValue == inCompareValue;
            break;
        }
        case GreaterThan:
        {
            return inPropertyValue > inCompareValue;
            break;
        }
        default:
            jassertfalse;
            return false;
            break;
    };
}

void SampleFileFilterRuleBase::setCompareOperator(CompareOperators inCompareOperator)
{
    mCompareOperator = inCompareOperator;
}
