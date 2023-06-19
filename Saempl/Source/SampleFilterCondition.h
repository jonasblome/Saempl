/*
  ==============================================================================

    SampleItemFilterCondition.h
    Created: 17 Jun 2023 3:02:11pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleItem.h"
#include "BlomeHelperFunctions.h"

class SampleFilterCondition
{
public:
    // Constructors
    SampleFilterCondition();
    SampleFilterCondition(String inNameTagToCheck, double inCompareValue, CompareOperator inCompareOperator);
    ~SampleFilterCondition();
    
    // Methods
    bool acceptsSampleItem(SampleItem* inSampleItem);
    void setNameTagToCheck(String inTagName);
    String getNameTagToCheck();
    void setCompareValue(double inCompareValue);
    double getCompareValue();
    void setCompareOperator(CompareOperator inCompareOperator);
    CompareOperator getCompareOperator();
    
private:
    // Fields
    String mTagNameToCheck;
    double mCompareValue;
    CompareOperator mCompareOperator;
    
    // Methods
    
};
