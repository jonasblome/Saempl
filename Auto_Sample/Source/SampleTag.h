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
    SampleTag();
    SampleTag(String inName, double inValue);
    ~SampleTag();
    
    void setName(String inName) { name = inName; }
    
    String getName() { return name; }
    
    void setValue(double inValue) { value = inValue; }
    
    double getValue() {return value; }
    
private:
    String name;
    double value;
};
