/*
 ==============================================================================
 
 SampleItem.h
 Created: 21 May 2023 3:01:25pm
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
     @returns the file path.
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
     @returns the samples length in seconds.
     */
    double getLength() const;
    /**
     Sets the length of the sample item in seconds.
     
     @param inLength the length to set.
     */
    void setLength(double inLength);
    /**
     @returns the samples loudness in dB.
     */
    double getLoudness() const;
    /**
     Sets the loudness of the sample item in dB.
     
     @param inLoudness the loudness to set.
     */
    void setLoudness(double inLoudness);
    
private:
    String mFilePath;
    String mTitle;
    double mLength;
    double mLoudness;
};
