/*
  ==============================================================================

    SamplePropertyInt.cpp
    Created: 26 Oct 2023 5:52:29pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SamplePropertyInt.h"

SamplePropertyInt::SamplePropertyInt()
{
    
}

SamplePropertyInt::SamplePropertyInt(String inName, int inValue)
{
    
}

SamplePropertyInt::~SamplePropertyInt()
{
    
}

void SamplePropertyInt::setValue(int inValue)
{
    mValue = inValue;
}

int SamplePropertyInt::getValue()
{
    return mValue;
}

int SamplePropertyInt::compareTo(SampleProperty& inProperty)
{
    int otherValue = dynamic_cast<SamplePropertyInt&>(inProperty).getValue();
    return (mValue < otherValue) ? -1 : ((mValue > otherValue) ? 1 : 0);
}
