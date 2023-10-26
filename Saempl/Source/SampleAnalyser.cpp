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

int SampleAnalyser::analyseProperty(int inPropertyName, File inFile)
{
    switch (inPropertyName)
    {
        case 0:
            return 0;
        case 1:
            return 0;
        case 2:
            return 0;
        case 3:
        {
            return analyseSampleLength(inFile);
        }
        case 4:
            return inFile.getFileNameWithoutExtension();
        default:
            return 0;
    }
}

int SampleAnalyser::analyseSampleLength(File inFile)
{
    loadAudioFileSource(inFile);
    int length = mCurrentAudioFileSource->getTotalLength() / mCurrentAudioFileSource->getAudioFormatReader()->sampleRate;
    
    return length;
}

void SampleAnalyser::loadAudioFileSource(File inFile)
{
    mCurrentAudioFileSource.reset();
    AudioFormatReader* reader = mFormatManager.createReaderFor(inFile);
    mCurrentAudioFileSource = std::make_unique<AudioFormatReaderSource>(reader, true);
}
