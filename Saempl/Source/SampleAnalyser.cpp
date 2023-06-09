/*
  ==============================================================================

    SampleAnalyser.cpp
    Created: 21 May 2023 3:00:58pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleAnalyser.h"

SampleAnalyser::SampleAnalyser()
{
    mFormatManager.registerBasicFormats();
}

SampleAnalyser::~SampleAnalyser()
{
    
}

double SampleAnalyser::analyseSampleLength(File inFile)
{
    loadAudioFileSource(inFile);
    
    return mCurrentAudioFileSource->getTotalLength() / mCurrentAudioFileSource->getAudioFormatReader()->sampleRate;
}

void SampleAnalyser::loadAudioFileSource(File inFile)
{
    mCurrentAudioFileSource.reset();
    AudioFormatReader* reader = mFormatManager.createReaderFor(inFile);
    mCurrentAudioFileSource = std::make_unique<AudioFormatReaderSource>(reader, true);
}
