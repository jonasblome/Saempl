/*
  ==============================================================================

    SampleProperty.h
    Created: 21 May 2023 3:01:29pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class SampleProperty
{
public:
    // Constructors
    SampleProperty();
    virtual ~SampleProperty();
    
    // Methods
    void setName(String const& inName);
    String getName();
    virtual int compareTo(SampleProperty& inProperty);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleProperty)
    
    // Fields
    String mName;
    
    // Methods
    
};
