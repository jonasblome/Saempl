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

String SampleItem::getFilePath() const
{
    return mFilePath;
}

void SampleItem::setFilePath(String const & inFilePath)
{
    mFilePath = inFilePath;
}

String SampleItem::getTitle() const
{
    return mTitle;
}

void SampleItem::setTitle(String inTitle)
{
    mTitle = inTitle;
}

double SampleItem::getLength() const
{
    return mLength;
}

void SampleItem::setLength(double inLength)
{
    mLength = inLength;
}

double SampleItem::getLoudness() const
{
    return mLoudness;
}

void SampleItem::setLoudness(double inLoudness)
{
    mLoudness = inLoudness;
}
