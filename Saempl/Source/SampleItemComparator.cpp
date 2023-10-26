/*
  ==============================================================================

    SampleItemComparator.cpp
    Created: 23 Oct 2023 10:12:45pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleItemComparator.h"

SampleItemComparator::SampleItemComparator()
{
    
}

SampleItemComparator::~SampleItemComparator()
{
    
}

int SampleItemComparator::compareElements(SampleItem* first, SampleItem* second)
{
    int result = first->getSampleProperty(mComparePropertyName)->compareTo(*second->getSampleProperty(mComparePropertyName));
    
    if (!mIsForwards)
    {
        result = result * (-1);
    }
    
    return result;
}

void SampleItemComparator::setCompareProperty(String inProperty)
{
    mComparePropertyName = inProperty;
}

void SampleItemComparator::setSortingDirection(bool inIsForwards)
{
    mIsForwards = inIsForwards;
}
