/*
  ==============================================================================

    SampleTag.h
    Created: 21 May 2023 3:01:29pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class SampleTag
{
public:
    // Constructors
    SampleTag();
    SampleTag(String inName, double inValue);
    ~SampleTag();
    
    // Methods
    void setName(String inTagName);
    String getName();
    void setValue(double inValue);
    double getValue();
    
private:
    // Fields
    String mTagName;
    double mTagValue;
    
    // Methods
    
};
