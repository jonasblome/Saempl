/*
  ==============================================================================

    SampleFileFilterRuleBase.cpp
    Created: 26 Oct 2023 10:50:12am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileFilterRuleBase.h"

SampleFileFilterRuleBase::SampleFileFilterRuleBase(String inRuleType)
:   mRuleType(inRuleType)
{
    
}

SampleFileFilterRuleBase::~SampleFileFilterRuleBase()
{
    
}

CompareOperators SampleFileFilterRuleBase::getCompareOperator()
{
    return mCompareOperator;
}

void SampleFileFilterRuleBase::setCompareOperator(CompareOperators inCompareOperator)
{
    mCompareOperator = inCompareOperator;
}

String SampleFileFilterRuleBase::getRuleType()
{
    return mRuleType;
}
