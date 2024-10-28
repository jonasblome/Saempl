/*
 ==============================================================================
 
 SampleItem.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "BlomeHelpers.h"

/**
 The class for storing meta information about a sample/audio file.
 
 Holds information on the file path, the title, length and more...
 */
class SampleItem
{
public:
    SampleItem();
    ~SampleItem();
    /**
     @returns the sample's file path.
     */
    String getFilePath() const;
    /**
     Sets the file path of the corresponding audio file.
     */
    void setFilePath(String const & inFilePath);
    /**
     @returns the sample's title.
     */
    String getTitle() const;
    /**
     Sets the sample items title.
     
     @param inTitle the title to set.
     */
    void setTitle(String inTitle);
    /**
     @returns the sample's length in seconds.
     */
    float getLength() const;
    /**
     Sets the length of the sample item in seconds.
     
     @param inLength the length to set.
     */
    void setLength(float inLength);
    /**
     @returns the sample's loudness in dB.
     */
    float getLoudnessDecibel() const;
    /**
     Sets the loudness of the sample item in dB.
     
     @param inDecibel the loudness to set.
     */
    void setLoudnessDecibel(float inDecibel);
    /**
     @returns the sample's loudness in LUFS.
     */
    float getLoudnessLUFS() const;
    /**
     Sets the loudness of the sample item in LUFS.
     
     @param inLUFS the loudness to set.
     */
    void setLoudnessLUFS(float inLUFS);
    /**
     @returns the sample's dynamic range in LUFS.
     */
    float getDynamicRange() const;
    /**
     Sets the dynamic range start of the sample item in LUFS.
     
     @param inDynamicRange the dynamic range to set.
     */
    void setDynamicRange(float inDynamicRange);
    /**
     @returns the sample's zero crossing rate.
     */
    float getZeroCrossingRate() const;
    /**
     Sets the zero crossing rate of the sample.
     
     @param inRate the loudness to set.
     */
    void setZeroCrossingRate(float inRate);
    /**
     @returns the sample's spectral centroid.
     */
    float getSpectralCentroid() const;
    /**
     Sets the spectral centroid of the sample.
     
     @param inSpectralCentroid the spectral centroid to set.
     */
    void setSpectralCentroid(float inSpectralCentroid);
    /**
     @returns the sample's spectral spread.
     */
    float getSpectralSpread() const;
    /**
     Sets the spectral spread of the sample.
     
     @param inSpectralSpread the spectral spread to set.
     */
    void setSpectralSpread(float inSpectralSpread);
    /**
     @returns the sample's spectral rolloff.
     */
    float getSpectralRolloff() const;
    /**
     Sets the spectral rolloff of the sample.
     
     @param inRolloff the rolloff to set.
     */
    void setSpectralRolloff(float inRolloff);
    /**
     @returns the sample's spectral flux.
     */
    float getSpectralFlux() const;
    /**
     Sets the spectral flux of the sample.
     
     @param inSpectralFlux the spectral flux to set.
     */
    void setSpectralFlux(float inSpectralFlux);
    /**
     @returns the sample's chroma flux.
     */
    float getChromaFlux() const;
    /**
     Sets the chroma flux of the sample.
     
     @param inChromaFlux the chroma flux to set.
     */
    void setChromaFlux(float inChromaFlux);
    /**
     @returns the sample's tempo in bpm.
     */
    int getTempo() const;
    /**
     Sets the tempo of the sample item in bpm.
     
     @param inTempo the tempo to set.
     */
    void setTempo(int inTempo);
    /**
     @returns the sample's key.
     */
    int getKey() const;
    /**
     Sets the key of the sample item.
     
     @param inKey the key to set.
     */
    void setKey(int inKey);
    /**
     @returns the sample's sample rate.
     */
    int getSampleRate() const;
    /**
     Sets the sample rate of the sample item.
     
     @param inSampleRate the key to set.
     */
    void setSampleRate(int inSampleRate);
    /**
     @returns the sample's spectral distribution.
     */
    std::vector<float> getSpectralDistribution() const;
    /**
     Sets the spectral distribution of the sample item.
     
     @param inSpectralDistribution the distribution to set.
     */
    void setSpectralDistribution(std::vector<float> inSpectralDistribution);
    /**
     @returns the sample's chromaDistribution.
     */
    std::vector<float> getChromaDistribution() const;
    /**
     Sets the chroma distribution of the sample item.
     
     @param inChromaDistribution the vector to set.
     */
    void setChromaDistribution(std::vector<float> inChromaDistribution);
    /**
     @returns the sample's feature vector.
     */
    std::vector<float> getFeatureVector() const;
    /**
     Sets the feature vector of the sample item.
     
     @param inFeatureVector the vector to set.
     */
    void setFeatureVector(std::vector<float> inFeatureVector);
    
private:
    String mFilePath;
    String mTitle;
    float mLength;
    float mLoudnessDecibel;
    float mLoudnessLUFS;
    float mDynamicRangeLUFS;
    float mZeroCrossingRate;
    float mSpectralCentroid;
    float mSpectralSpread;
    float mSpectralRolloff;
    float mSpectralFlux;
    float mChromaFlux;
    int mSampleRate;
    int mTempo;
    int mKey;
    std::vector<float> mSpectralDistribution;
    std::vector<float> mChromaDistribution;
    std::vector<float> mFeatureVector;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleItem);
};
