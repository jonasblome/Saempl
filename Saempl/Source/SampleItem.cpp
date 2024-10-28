/*
 ==============================================================================
 
 SampleItem.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleItem.h"

SampleItem::SampleItem()
{
    mTitle = EMPTY_TILE_PATH;
    mCurrentFilePath = EMPTY_TILE_PATH;
    mKey = NO_KEY_INDEX;
    mTempo = 0;
    mFeatureVector = std::vector<float>(NUM_CHROMA + NUM_SPECTRAL_BANDS + NUM_FEATURES);
    mSpectralDistribution = std::vector<float>(NUM_SPECTRAL_BANDS);
    mChromaDistribution = std::vector<float>(NUM_CHROMA);
}

SampleItem::~SampleItem()
{
    
}

String SampleItem::getCurrentFilePath() const
{
    return mCurrentFilePath;
}

void SampleItem::setCurrentFilePath(String const & inFilePath)
{
    mCurrentFilePath = inFilePath;
}

String SampleItem::getOldFilePath() const
{
    return mOldFilePath;
}

void SampleItem::setOldFilePath(String const & inFilePath)
{
    mOldFilePath = inFilePath;
}

String SampleItem::getTitle() const
{
    return mTitle;
}

void SampleItem::setTitle(String inTitle)
{
    mTitle = inTitle;
}

float SampleItem::getLength() const
{
    return mLength;
}

void SampleItem::setLength(float inLength)
{
    mLength = inLength;
}

float SampleItem::getLoudnessDecibel() const
{
    return mLoudnessDecibel;
}

void SampleItem::setLoudnessDecibel(float inDB)
{
    mLoudnessDecibel = inDB;
}

float SampleItem::getLoudnessLUFS() const
{
    return mLoudnessLUFS;
}

void SampleItem::setLoudnessLUFS(float inLUFS)
{
    mLoudnessLUFS = inLUFS;
}

float SampleItem::getDynamicRange() const
{
    return mDynamicRangeLUFS;
}

void SampleItem::setDynamicRange(float inDynamicRange)
{
    mDynamicRangeLUFS = inDynamicRange;
}
float SampleItem::getZeroCrossingRate() const
{
    return mZeroCrossingRate;
}

void SampleItem::setZeroCrossingRate(float inRate)
{
    mZeroCrossingRate = inRate;
}

float SampleItem::getSpectralCentroid() const
{
    return mSpectralCentroid;
}

void SampleItem::setSpectralCentroid(float inSpectralCentroid)
{
    mSpectralCentroid = inSpectralCentroid;
}

float SampleItem::getSpectralSpread() const
{
    return mSpectralSpread;
}

void SampleItem::setSpectralSpread(float inSpectralSpread)
{
    mSpectralSpread = inSpectralSpread;
}

float SampleItem::getSpectralRolloff() const
{
    return mSpectralRolloff;
}

void SampleItem::setSpectralRolloff(float inRolloff)
{
    mSpectralRolloff = inRolloff;
}

float SampleItem::getSpectralFlux() const
{
    return mSpectralFlux;
}

void SampleItem::setSpectralFlux(float inSpectralFlux)
{
    mSpectralFlux = inSpectralFlux;
}

float SampleItem::getChromaFlux() const
{
    return mChromaFlux;
}

void SampleItem::setChromaFlux(float inChromaFlux)
{
    mChromaFlux = inChromaFlux;
}

std::vector<float> SampleItem::getSpectralDistribution() const
{
    return mSpectralDistribution;
}

void SampleItem::setSpectralDistribution(std::vector<float> inSpectralDistribution)
{
    mSpectralDistribution = inSpectralDistribution;
}

std::vector<float> SampleItem::getChromaDistribution() const
{
    return mChromaDistribution;
}

void SampleItem::setChromaDistribution(std::vector<float> inChromaDistribution)
{
    mChromaDistribution = inChromaDistribution;
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

int SampleItem::getSampleRate() const
{
    return mSampleRate;
}

void SampleItem::setSampleRate(int inSampleRate)
{
    mSampleRate = inSampleRate;
}

std::vector<float> SampleItem::getFeatureVector() const
{
    return mFeatureVector;
}

void SampleItem::setFeatureVector(std::vector<float> inFeatureVector)
{
    mFeatureVector = inFeatureVector;
}
