/*
 ==============================================================================
 
 SampleItem.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"

/**
 The class for storing meta information about a sample/audio file.
 
 Holds information on the file path, the title, length and more in the future.
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
    double getLength() const;
    /**
     Sets the length of the sample item in seconds.
     
     @param inLength the length to set.
     */
    void setLength(double inLength);
    /**
     @returns the sample's loudness in dB.
     */
    double getLoudnessDecibel() const;
    /**
     Sets the loudness of the sample item in dB.
     
     @param inLoudness the loudness to set.
     */
    void setLoudnessDecibel(double inDB);
    /**
     @returns the sample's loudness in LUFS.
     */
    double getLoudnessLUFS() const;
    /**
     Sets the loudness of the sample item in LUFS.
     
     @param inLoudness the loudness to set.
     */
    void setLoudnessLUFS(double inLUFS);
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
    String getKey() const;
    /**
     Sets the key of the sample item.
     
     @param inKey the key to set.
     */
    void setKey(String inKey);
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
    double mLength;
    double mLoudnessDecibel;
    double mLoudnessLUFS;
    int mTempo;
    String mKey;
    std::vector<float> mFeatureVector;
};
