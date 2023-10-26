/*
  ==============================================================================

    SamplePropertyDouble.h
    Created: 26 Oct 2023 5:52:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleProperty.h"

class SamplePropertyDouble
:   public SampleProperty
{
public:
    // Constructors
    SamplePropertyDouble();
    SamplePropertyDouble(String inName, double inValue);
    ~SamplePropertyDouble();
    
    // Methods
    double getValue();
    void setValue(double inValue);
    int compareTo(SampleProperty& inProperty) override;
    
private:
    // Fields
    double mValue;
    
    // Methods
    
};
