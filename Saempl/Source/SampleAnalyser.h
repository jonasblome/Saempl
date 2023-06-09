/*
  ==============================================================================

    SampleAnalyser.h
    Created: 21 May 2023 3:00:58pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class SampleAnalyser
{
public:
    // Constructors
    SampleAnalyser();
    ~SampleAnalyser();
    
    // Methods
    double analyseSampleLength(File inFile);
    void loadAudioFileSource(File inFile);
    
private:
    // Fields
    std::unique_ptr<AudioFormatReaderSource> mCurrentAudioFileSource;
    AudioFormatManager mFormatManager;
    
    // Methods
    
};
