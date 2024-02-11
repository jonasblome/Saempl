/*
  ==============================================================================

    SampleFileFilterRuleLoudness.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileFilterRuleLoudnessLUFS.h"

SampleFileFilterRuleLoudnessLUFS::SampleFileFilterRuleLoudnessLUFS(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareValue = -300.0;
    mCompareOperator = GREATER_THAN;
}

SampleFileFilterRuleLoudnessLUFS::~SampleFileFilterRuleLoudnessLUFS()
{
    
}

bool SampleFileFilterRuleLoudnessLUFS::matches(SampleItem const & inSampleItem)
{
    int propertyValue = inSampleItem.getLoudnessLUFS();
    
    switch (mCompareOperator) {
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

double SampleFileFilterRuleLoudnessLUFS::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleLoudnessLUFS::setCompareValue(double const & inCompareValue)
{
    mCompareValue = inCompareValue;
}

bool SampleFileFilterRuleLoudnessLUFS::canHaveEffect()
{
    return isActive && (mCompareOperator != GREATER_THAN || mCompareValue != -300);
}
