/*
  ==============================================================================

    SamplePropertyString.cpp
    Created: 26 Oct 2023 5:53:02pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SamplePropertyString.h"

SamplePropertyString::SamplePropertyString()
{
    
}

SamplePropertyString::SamplePropertyString(String inName, String inValue)
{
    
}

SamplePropertyString::~SamplePropertyString()
{
    
}

void SamplePropertyString::setValue(String inValue)
{
    mValue = inValue;
}

String SamplePropertyString::getValue()
{
    return mValue;
}

int SamplePropertyString::compareTo(SampleProperty& inProperty)
{
    String otherValue = dynamic_cast<SamplePropertyString&>(inProperty).getValue();
    return (mValue < otherValue) ? -1 : ((mValue > otherValue) ? 1 : 0);
}
