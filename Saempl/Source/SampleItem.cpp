/*
 ==============================================================================
 
 SampleItem.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleItem.h"

SampleItem::SampleItem()
{
    mTitle = "EMPTYTILE";
    mFilePath = "EMPTYTILE";
    mKey = NO_KEY_INDEX;
    mTempo = 0;
    mFeatureVector = std::vector<float>(NUM_CHROMA + NUM_SPECTRAL_BANDS + NUM_FEATURES);
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

int SampleItem::getKey() const
{
    return mKey;
}

void SampleItem::setKey(int inKey)
{
    mKey = inKey;
}

std::vector<float> SampleItem::getFeatureVector() const
{
    return mFeatureVector;
}

void SampleItem::setFeatureVector(std::vector<float> inFeatureVector)
{
    mFeatureVector = inFeatureVector;
}
