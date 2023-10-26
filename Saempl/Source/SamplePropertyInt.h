/*
  ==============================================================================

    SamplePropertyInt.h
    Created: 26 Oct 2023 5:52:29pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleProperty.h"

class SamplePropertyInt
:   public SampleProperty
{
public:
    // Constructors
    SamplePropertyInt();
    SamplePropertyInt(String inName, int inValue);
    ~SamplePropertyInt();
    
    // Methods
    int getValue();
    void setValue(int inValue);
    int compareTo(SampleProperty& inProperty) override;
    
private:
    // Fields
    int mValue;
    
    // Methods
    
};
