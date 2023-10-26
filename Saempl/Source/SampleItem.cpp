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

SampleProperty* SampleItem::addSampleProperty(SampleProperty* inSampleProperty)
{
    return mSampleProperties.add(inSampleProperty);
}

SampleProperty* SampleItem::getSampleProperty(String& inPropertyName)
{
    for(SampleProperty* property : mSampleProperties)
    {
        if (property->getName() == inPropertyName) {
            return property;
        }
    }
    
    return nullptr;
}

SampleProperty* SampleItem::getSampleProperty(int inPropertyIndex)
{
    return mSampleProperties.getUnchecked(inPropertyIndex);
}

OwnedArray<SampleProperty>* SampleItem::getSampleProperties()
{
    return &mSampleProperties;
}

void SampleItem::setFilePath(String const & inFilePath)
{
    mFilePath = inFilePath;
}

String SampleItem::getFilePath()
{
    return mFilePath;
}

String SampleItem::getTitle()
{
    File itemFile = mFilePath;
    String itemTitle = itemFile.getFileNameWithoutExtension();
    return itemTitle;
}
