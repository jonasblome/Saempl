/*
 ==============================================================================
 
 SampleItemComparator.cpp
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
    if (mComparePropertyName == PROPERTY_NAMES[0])
    {
        return compareElements<String>(first->getTitle(), second->getTitle());
    }
    else if (mComparePropertyName == PROPERTY_NAMES[1])
    {
        return compareElements<float>(first->getLength(), second->getLength());
    }
    else if (mComparePropertyName == PROPERTY_NAMES[2])
    {
        return compareElements<float>(first->getLoudnessDecibel(), second->getLoudnessDecibel());
    }
    else if (mComparePropertyName == PROPERTY_NAMES[3])
    {
        return compareElements<float>(first->getLoudnessLUFS(), second->getLoudnessLUFS());
    }
    else if (mComparePropertyName == PROPERTY_NAMES[4])
    {
        return compareElements<int>(first->getTempo(), second->getTempo());
    }
    else if (mComparePropertyName == PROPERTY_NAMES[5])
    {
        return compareElements<int>(first->getKey(), second->getKey());
    }
    
    return 0;
}

template <typename T> int SampleItemComparator::compareElements(T first, T second)
{
    int result = first < second ? -1 : (first > second ? 1 : 0);
    
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
