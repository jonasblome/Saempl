/*
  ==============================================================================

    SampleProperty.cpp
    Created: 21 May 2023 3:01:29pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleProperty.h"

SampleProperty::SampleProperty()
{
    
}

SampleProperty::~SampleProperty()
{
    
}

void SampleProperty::setName(String const & inName)
{
    mName = inName;
}

String SampleProperty::getName()
{
    return mName;
}
