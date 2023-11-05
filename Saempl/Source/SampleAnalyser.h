/*
  ==============================================================================

    SampleAnalyser.h
    Created: 21 May 2023 3:00:58pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

/**
 Analyses sample files for given categories.
 */
class SampleAnalyser
{
public:
    SampleAnalyser();
    ~SampleAnalyser();
    /**
     Analyses the length of the given file in seconds.
     
     @param inFile the file to analyse.
     
     @returns the length of the audio file in seconds.
     */
    double analyseSampleLength(File inFile);
    
private:
    std::unique_ptr<AudioFormatReaderSource> mCurrentAudioFileSource;
    std::unique_ptr<AudioFormatManager> mFormatManager;
    
    /**
     Loads the given file into a audio source.
     
     @param inFile the file to load into the audio file source.
     */
    void loadAudioFileSource(File inFile);
};
