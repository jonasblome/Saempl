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
    // Constructors
    SampleItem();
    ~SampleItem();
    
    // Methods
    void addSampleTag(SampleTag inSampleTag);
    void setURL(URL inURL);
    
private:
    // Fields
    OwnedArray<SampleTag> mSampleTags;
    URL mURL;
};
