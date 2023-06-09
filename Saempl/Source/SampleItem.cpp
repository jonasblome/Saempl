/*
  ==============================================================================

    SampleItem.cpp
    Created: 21 May 2023 3:01:25pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleItem.h"

SampleItem::SampleItem()
{
    
}

SampleItem::~SampleItem()
{
    
}

void SampleItem::addSampleTag(SampleTag inSampleTag)
{
    mSampleTags.add(&inSampleTag);
}

void SampleItem::setURL(URL inURL)
{
    mURL = inURL;
}
