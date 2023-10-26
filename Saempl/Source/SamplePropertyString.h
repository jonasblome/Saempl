/*
  ==============================================================================

    SamplePropertyString.h
    Created: 26 Oct 2023 5:53:02pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleProperty.h"

class SamplePropertyString
:   public SampleProperty
{
public:
    // Constructors
    SamplePropertyString();
    SamplePropertyString(String inName, String inValue);
    ~SamplePropertyString();
    
    // Methods
    String getValue();
    void setValue(String inValue);
    int compareTo(SampleProperty& inProperty) override;
    
private:
    // Fields
    String mValue;
    
    // Methods
    
};
