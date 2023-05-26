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
    name = inName;
    value = inValue;
}

SampleTag::~SampleTag()
{
}
