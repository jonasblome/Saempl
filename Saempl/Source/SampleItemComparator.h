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

class SampleItemComparator
{
public:
    // Constructors
    SampleItemComparator();
    ~SampleItemComparator();
    
    // Methods
    int compareElements (SampleItem* first, SampleItem* second);
    template <typename T> int compareElements(T first, T second);
    void setCompareProperty(String inPropertyName);
    void setSortingDirection(bool isForwards);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleItemComparator)
    
    // Fields
    String mComparePropertyName;
    bool mIsForwards;
};
