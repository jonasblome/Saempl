/*
  ==============================================================================

    SampleItem.h
    Created: 21 May 2023 3:01:25pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleProperty.h"

class SampleItem
{
public:
    // Constructors
    SampleItem();
    ~SampleItem();
    
    // Methods
    SampleProperty* addSampleProperty(SampleProperty* inSampleProperty);
    SampleProperty* getSampleProperty(String& inPropertyName);
    SampleProperty* getSampleProperty(int inPropertyIndex);
    OwnedArray<SampleProperty>* getSampleProperties();
    void setFilePath(String const & inFilePath);
    String getFilePath();
    String getTitle();
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleItem)
    
    // Fields
    OwnedArray<SampleProperty> mSampleProperties;
    String mFilePath;
    
    // Methods
    
};
