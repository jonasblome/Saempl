/*
 ==============================================================================
 
 SampleFileFilterRuleComment.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFileFilterRuleComment.h"

SampleFileFilterRuleComment::SampleFileFilterRuleComment(String inRulePropertyName)
:
SampleFileFilterRuleBase(inRulePropertyName)
{
    mCompareOperator = CONTAINS;
}

SampleFileFilterRuleComment::~SampleFileFilterRuleComment()
{
    
}

bool SampleFileFilterRuleComment::matches(SampleItem const& inSampleItem)
{
    String propertyValue = inSampleItem.getComment();
    
    switch (mCompareOperator)
    {
        case LESS_THAN:
        {
            return false;
            break;
        }
        case EQUAL_TO:
        {
            return propertyValue == mCompareValue;
            break;
        }
        case GREATER_THAN:
        {
            return false;
            break;
        }
        case CONTAINS:
        {
            return propertyValue.containsIgnoreCase(mCompareValue);
        }
        default:
        {
            jassertfalse;
            return false;
        }
    };
}

String SampleFileFilterRuleComment::getCompareValue()
{
    return mCompareValue;
}

void SampleFileFilterRuleComment::setCompareValue(String const& inCompareValue)
{
    mCompareValue = inCompareValue;
}

bool SampleFileFilterRuleComment::canHaveEffect()
{
    return isActive && (mCompareOperator != CONTAINS || mCompareValue != "");
}
