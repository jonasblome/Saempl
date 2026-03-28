/*
 ==============================================================================
 
 SampleItem.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleItem.h"

SampleItem::SampleItem()
{
    mCurrentFilePath = EMPTY_TILE_PATH;
    mOldFilePath = EMPTY_TILE_PATH;
    mTitle = EMPTY_TILE_PATH;
    mLength = 0;
    mLoudnessDecibel = MINIMUM_LOUDNESS;
    mLoudnessLUFS = MINIMUM_LOUDNESS;
    mZeroCrossingRate = 0;
    mSpectralCentroid = 0;
    mSpectralSpread = 0;
    mSpectralRolloff = 0;
    mSpectralFlux = 0;
    mChromaFlux = 0;
    mSampleRate = 0;
    mTempo = 0;
    mKey = NO_KEY_INDEX;
    mSpectralDistribution = std::vector<float>(NUM_SPECTRAL_BANDS);
    mChromaDistribution = std::vector<float>(NUM_CHROMA);
    mFeatureVector = std::vector<float>(NUM_CHROMA + NUM_SPECTRAL_BANDS + NUM_FEATURES);
    mComment = String();
    mPropertyLock = false;
    mLockedProperties = std::set<String>();
}

SampleItem::~SampleItem()
{
    
}

String SampleItem::getCurrentFilePath() const
{
    return mCurrentFilePath;
}

void SampleItem::setCurrentFilePath(String const& inFilePath)
{
    mCurrentFilePath = inFilePath;
}

String SampleItem::getOldFilePath() const
{
    return mOldFilePath;
}

void SampleItem::setOldFilePath(String const& inFilePath)
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

String SampleItem::getComment() const
{
    return mComment;
}

void SampleItem::setComment(String inComment)
{
    mComment = inComment;
}

bool SampleItem::getPropertyLock() const
{
    return mPropertyLock;
}

void SampleItem::setPropertyLock(bool inIsLocked)
{
    mPropertyLock = inIsLocked;
}

std::vector<float> SampleItem::getFeatureVector() const
{
    return mFeatureVector;
}

void SampleItem::setFeatureVector(std::vector<float> inFeatureVector)
{
    mFeatureVector = inFeatureVector;
}

void SampleItem::clearLockedProperties()
{
    mLockedProperties.clear();
}

void SampleItem::addLockedProperty(String inPropertyName)
{
    mLockedProperties.insert(inPropertyName);
}

std::set<String> SampleItem::getLockedProperties()
{
    return mLockedProperties;
}
