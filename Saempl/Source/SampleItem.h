/*
  ==============================================================================

    SampleItem.h
    Created: 21 May 2023 3:01:25pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleTag.h"

class SampleItem
{
public:
    // Constructors
    SampleItem();
    ~SampleItem();
    
    // Methods
    void addSampleTag(SampleTag* inSampleTag);
    SampleTag* getSampleTag(String& inTagName);
    SampleTag* getSampleTag(int inTagIndex);
    OwnedArray<SampleTag>* getSampleTags();
    void setFilePath(const String& inFilePath);
    String getFilePath();
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleItem)
    
    // Fields
    OwnedArray<SampleTag> mSampleTags;
    String mFilePath;
    
    // Methods
    
};
