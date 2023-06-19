/*
  ==============================================================================

    SampleFileFilterCondition.cpp
    Created: 17 Jun 2023 3:02:11pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFilterCondition.h"

SampleFilterCondition::SampleFilterCondition()
{
    
}

SampleFilterCondition::SampleFilterCondition(String inNameTagToCheck, double inCompareValue, CompareOperator inCompareOperator)
{
    mTagNameToCheck = inNameTagToCheck;
    mCompareValue = inCompareValue;
    mCompareOperator = inCompareOperator;
}

SampleFilterCondition::~SampleFilterCondition()
{
    
}

bool SampleFilterCondition::acceptsSampleItem(SampleItem* inSampleItem)
{
    switch (mCompareOperator) {
        case     CompareOperator_Equals:
            return inSampleItem->getSampleTag(mTagNameToCheck)->getValue() == mCompareValue;
        case     CompareOperator_LessThan:
            return inSampleItem->getSampleTag(mTagNameToCheck)->getValue() < mCompareValue;
        case     CompareOperator_BiggerThan:
            return inSampleItem->getSampleTag(mTagNameToCheck)->getValue() > mCompareValue;
        default:
            return false;
    }
}

void SampleFilterCondition::setNameTagToCheck(String inTagName)
{
    mTagNameToCheck = inTagName;
}

String SampleFilterCondition::getNameTagToCheck()
{
    return mTagNameToCheck;
}

void SampleFilterCondition::setCompareOperator(CompareOperator inCompareOperator)
{
    mCompareOperator = inCompareOperator;
}

CompareOperator SampleFilterCondition::getCompareOperator()
{
    return mCompareOperator;
}

void SampleFilterCondition::setCompareValue(double inCompareValue)
{
    mCompareValue = inCompareValue;
}

double SampleFilterCondition::getCompareValue()
{
    return mCompareValue;
}
