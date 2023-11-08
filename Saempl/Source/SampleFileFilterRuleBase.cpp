/*
  ==============================================================================

    SampleFileFilterRuleBase.cpp
    Created: 26 Oct 2023 10:50:12am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileFilterRuleBase.h"

SampleFileFilterRuleBase::SampleFileFilterRuleBase(String inRulePropertyName)
:   mRulePropertyName(inRulePropertyName)
{
    isActive = true;
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

String SampleFileFilterRuleBase::getRulePropertyName()
{
    return mRulePropertyName;
}

bool SampleFileFilterRuleBase::getIsActive()
{
    return isActive;
}

void SampleFileFilterRuleBase::setIsActive(bool inIsActive)
{
    isActive = inIsActive;
}