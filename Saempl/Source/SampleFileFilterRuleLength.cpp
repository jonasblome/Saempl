/*
  ==============================================================================

    SampleFileFilterRuleLength.cpp
    Created: 28 Oct 2023 5:24:42pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileFilterRuleLength.h"

SampleFileFilterRuleLength::SampleFileFilterRuleLength(String inRuleType)
:   SampleFileFilterRuleBase(inRuleType)
{
    mCompareValue = 0.0;
    mCompareOperator = GREATER_THAN;
}

SampleFileFilterRuleLength::~SampleFileFilterRuleLength()
{
    
}

bool SampleFileFilterRuleLength::matches(SampleItem const & inSampleItem)
{
    double propertyValue = inSampleItem.getLength();
    
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
        default:
            jassertfalse;
            return false;
    };
}

double SampleFileFilterRuleLength::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleLength::setCompareValue(double const & inCompareValue)
{
    mCompareValue = inCompareValue;
}
