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
    if (mComparePropertyName == "Title")
    {
        return compareElements<String>(first->getTitle(), second->getTitle());
    }
    else if (mComparePropertyName == "Length")
    {
        return compareElements<double>(first->getLength(), second->getLength());
    }
    
    return 0;
}

template <typename T> int SampleItemComparator::compareElements(T first, T second)
{
    int result = first < second ? -1 : first > second ? 1 : 0;
    
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
