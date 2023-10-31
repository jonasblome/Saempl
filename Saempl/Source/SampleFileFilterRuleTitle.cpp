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
    mCompareOperator = Contains;
}

SampleFileFilterRuleTitle::~SampleFileFilterRuleTitle()
{
    
}

bool SampleFileFilterRuleTitle::matches(SampleItem const & inSampleItem)
{
    String propertyValue = inSampleItem.getTitle();
    
    switch (mCompareOperator) {
        case LessThan:
        {
            return propertyValue > mCompareValue;
            break;
        }
        case EqualTo:
        {
            return propertyValue == mCompareValue;
            break;
        }
        case GreaterThan:
        {
            return propertyValue < mCompareValue;
            break;
        }
        case Contains:
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
