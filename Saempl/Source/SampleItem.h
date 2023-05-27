/*
  ==============================================================================

    SampleItem.h
    Created: 21 May 2023 3:01:25pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleTag.h"

class SampleItem
{
public:
    SampleItem();
    ~SampleItem();
    
private:
    OwnedArray<SampleTag> mSampleTags;
    String mUrl;
};
