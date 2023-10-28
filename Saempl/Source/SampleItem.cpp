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
    return mTitle;
}

void SampleItem::setTitle(String inTitle)
{
    mTitle = inTitle;
}

double SampleItem::getLength()
{
    return mLength;
}

void SampleItem::setLength(double inLength)
{
    mLength = inLength;
}
