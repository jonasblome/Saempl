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
#include "BlomeHelpers.h"

/**
 Compares \SampleItem objects given a certain property and a sorting direction.
 
 @see BlomeTableViewBase
 */
class SampleItemComparator
{
public:
    SampleItemComparator();
    ~SampleItemComparator();
    /**
     Compare the set property of the first and second sample item.
     Flipped results if sorting direction is backwards.
     
     @param first the first sample item to compare.
     @param second the second sample item to compare.
     
     @returns -1 if the property of first is smaller than seconds, 0 if equal and 1 if first is bigger than second.
     */
    int compareElements (SampleItem* first, SampleItem* second);
    /**
     Sets the name of the property over which to compare the two sample items.
     
     @param inPropertyName the name of the property to compare the sample items over.
     */
    void setCompareProperty(String inPropertyName);
    /**
     Sets the sorting direction to the given value.
     
     @param isForwards the sorting direction to set.
     */
    void setSortingDirection(bool isForwards);
    
private:
    String mComparePropertyName;
    bool mIsForwards;
    
    /**
     @see compareElements()
     */
    template <typename T> int compareElements(T first, T second);
};
