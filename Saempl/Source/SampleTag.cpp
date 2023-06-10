/*
  ==============================================================================

    SampleTag.cpp
    Created: 21 May 2023 3:01:29pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleTag.h"

SampleTag::SampleTag()
{
    
}

SampleTag::SampleTag(String inName, double inValue)
{
    mTagName = inName;
    mTagValue = inValue;
}

SampleTag::~SampleTag()
{
    
}

void SampleTag::setName(String inTagName)
{
    mTagName = inTagName;
}

String SampleTag::getName()
{
    return mTagName;
}

void SampleTag::setValue(double inValue)
{
    mTagValue = inValue;
}

double SampleTag::getValue()
{
    return mTagValue;
}
