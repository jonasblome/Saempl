/*
  ==============================================================================

    SampleFileFilterRuleTitle.cpp
    Created: 28 Oct 2023 5:24:27pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileFilterRuleTitle.h"

SampleFileFilterRuleTitle::SampleFileFilterRuleTitle(String inRuleType)
:   SampleFileFilterRuleBase(inRuleType)
{
    mCompareValue = "";
    mCompareOperator = CONTAINS;
}

SampleFileFilterRuleTitle::~SampleFileFilterRuleTitle()
{
    
}

bool SampleFileFilterRuleTitle::matches(SampleItem const & inSampleItem)
{
    String propertyValue = inSampleItem.getTitle();
    
    switch (mCompareOperator) {
        case LESS_THAN:
        {
            return propertyValue > mCompareValue;
            break;
        }
        case EQUAL_TO:
        {
            return propertyValue == mCompareValue;
            break;
        }
        case GREATER_THAN:
        {
            return propertyValue < mCompareValue;
            break;
        }
        case CONTAINS:
        {
            return propertyValue.toLowerCase().contains(mCompareValue.toLowerCase());
        }
        default:
            jassertfalse;
            return false;
    };
}

String SampleFileFilterRuleTitle::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleTitle::setCompareValue(String const & inCompareValue)
{
    mCompareValue = inCompareValue;
}
