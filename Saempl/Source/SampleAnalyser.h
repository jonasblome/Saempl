/*
 ==============================================================================
 
 SampleAnalyser.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "Ebu128LoudnessMeter.h"

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
    double analyseSampleLength(File const & inFile);
    /**
     Analyses the loudness of the given file in dB.
     
     @param inFile the file to analyse.
     
     @returns the loudness of the audio file in dB.
     */
    double analyseSampleLoudnessDecibel(File const & inFile);
    /**
     Analyses the loudness of the given file in LUFS.
     
     @param inFile the file to analyse.
     
     @returns the loudness of the audio file in LUFS.
     */
    double analyseSampleLoudnessLUFS(File const & inFile);
    
private:
    std::unique_ptr<AudioFormatReaderSource> mCurrentAudioFileSource;
    std::unique_ptr<AudioFormatManager> mFormatManager;
    int bufferSize = 512;
    AudioBuffer<float> analysisBuffer = AudioBuffer<float>(5, bufferSize);
    Ebu128LoudnessMeter ebuLoudnessMeter;
    
    /**
     Loads the given file into a audio source.
     
     @param inFile the file to load into the audio file source.
     */
    void loadAudioFileSource(File const & inFile);
};
