/*
 ==============================================================================
 
 SampleItem.cpp
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

double SampleItem::getLoudnessDecibel() const
{
    return mLoudnessDecibel;
}

void SampleItem::setLoudnessDecibel(double inDB)
{
    mLoudnessDecibel = inDB;
}

double SampleItem::getLoudnessLUFS() const
{
    return mLoudnessLUFS;
}

void SampleItem::setLoudnessLUFS(double inLUFS)
{
    mLoudnessLUFS = inLUFS;
}

int SampleItem::getTempo() const
{
    return mTempo;
}

void SampleItem::setTempo(int inTempo)
{
    mTempo = inTempo;
}

String SampleItem::getKey() const
{
    return mKey;
}

void SampleItem::setKey(String inKey)
{
    mKey = inKey;
}
