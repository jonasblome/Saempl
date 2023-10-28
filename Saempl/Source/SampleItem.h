/*
  ==============================================================================

    SampleItem.h
    Created: 21 May 2023 3:01:25pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class SampleItem
{
public:
    // Constructors
    SampleItem();
    ~SampleItem();
    
    // Methods
    void setFilePath(String const & inFilePath);
    String getFilePath();
    String getTitle();
    void setTitle(String inTitle);
    double getLength();
    void setLength(double inLength);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleItem)
    
    // Fields
    String mFilePath;
    String mTitle;
    double mLength;
    
    // Methods
    
};
