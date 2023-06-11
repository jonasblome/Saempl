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

void SampleItem::addSampleTag(SampleTag* inSampleTag)
{
    mSampleTags.add(inSampleTag);
}

SampleTag* SampleItem::getSampleTag(String inTagName)
{
    for(SampleTag* tag : mSampleTags)
    {
        if (tag->getName() == inTagName) {
            return tag;
        }
    }
    
    return nullptr;
}

SampleTag* SampleItem::getSampleTag(int inTagIndex)
{
    return mSampleTags.getUnchecked(inTagIndex);
}

OwnedArray<SampleTag>* SampleItem::getSampleTags()
{
    return &mSampleTags;
}

void SampleItem::setFilePath(String inFilePath)
{
    mFilePath = inFilePath;
}

String SampleItem::getFilePath()
{
    return mFilePath;
}
