/*
  ==============================================================================

    SamplePropertyDouble.cpp
    Created: 26 Oct 2023 5:52:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SamplePropertyDouble.h"

SamplePropertyDouble::SamplePropertyDouble()
{
    
}

SamplePropertyDouble::SamplePropertyDouble(String inName, double inValue)
{
    
}

SamplePropertyDouble::~SamplePropertyDouble()
{
    
}

void SamplePropertyDouble::setValue(double inValue)
{
    mValue = inValue;
}

double SamplePropertyDouble::getValue()
{
    return mValue;
}

int SamplePropertyDouble::compareTo(SampleProperty& inProperty)
{
    double otherValue = dynamic_cast<SamplePropertyDouble&>(inProperty).getValue();
    return (mValue < otherValue) ? -1 : ((mValue > otherValue) ? 1 : 0);
}
