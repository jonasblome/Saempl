/*
  ==============================================================================

    SampleItemComparator.h
    Created: 23 Oct 2023 10:12:45pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"

class SampleItemComparator
{
public:
    // Constructors
    SampleItemComparator();
    ~SampleItemComparator();
    
    // Methods
    int compareElements (SampleItem* first, SampleItem* second);
    template <typename T> int compareElements(T first, T second);
    void setCompareCategory(String inCategoryName);
    void setSortingDirection(bool isForwards);
    
private:
    // Fields
    String mCompareCategoryName;
    bool mIsForwards;
};
