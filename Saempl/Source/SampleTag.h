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
    
    void setName(String inName) { mTagName = inName; }
    
    String getName() { return mTagName; }
    
    void setValue(double inValue) { mTagValue = inValue; }
    
    double getValue() { return mTagValue; }
    
private:
    String mTagName;
    double mTagValue;
};
