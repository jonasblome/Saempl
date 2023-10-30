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
