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
    if (mCompareCategoryName == "Title")
    {
        String firstTitle = first->getTitle();
        String secondTitle = second->getTitle();
        return compareElements(firstTitle, secondTitle);
    }
    else
    {
        double firstTagValue = first->getSampleTag(mCompareCategoryName)->getValue();
        double secondTagValue = second->getSampleTag(mCompareCategoryName)->getValue();
        return compareElements(firstTagValue, secondTagValue);
    }
}

template <typename T> int SampleItemComparator::compareElements(T first, T second)
{
    int result =  (first < second) ? -1 : ((first > second) ? 1 : 0);
    
    if (!mIsForwards)
    {
        result = result * (-1);
    }
    
    return result;
}

void SampleItemComparator::setCompareCategory(String inCategory)
{
    mCompareCategoryName = inCategory;
}

void SampleItemComparator::setSortingDirection(bool isForwards)
{
    mIsForwards = isForwards;
}
