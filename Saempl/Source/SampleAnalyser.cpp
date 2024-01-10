/*
 ==============================================================================
 
 SampleAnalyser.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleAnalyser.h"

SampleAnalyser::SampleAnalyser()
{
    mFormatManager = std::make_unique<AudioFormatManager>();
    mFormatManager->registerBasicFormats();
}

SampleAnalyser::~SampleAnalyser()
{
    
}

double SampleAnalyser::analyseSampleLength(File const & inFile)
{
    loadAudioFileSource(inFile);
    double length = mCurrentAudioFileSource->getTotalLength() / mCurrentAudioFileSource->getAudioFormatReader()->sampleRate;
    
    return length;
}

double SampleAnalyser::analyseSampleLoudnessDecibel(const File &inFile)
{
    loadAudioFileSource(inFile);
    int numChannels = mCurrentAudioFileSource->getAudioFormatReader()->numChannels;
    int64 totalNumSamples = mCurrentAudioFileSource->getTotalLength();
    int numBlocks = int (totalNumSamples / bufferSize) + 1;
    int lastBlockLength = totalNumSamples % bufferSize;
    double decibel = 0.0;
    
    for (int b = 0; b < numBlocks; b++)
    {
        mCurrentAudioFileSource->getAudioFormatReader()->read(&analysisBuffer, 0, bufferSize, b * bufferSize, true, true);
        double decibelSumBlock = 0.0;
        
        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto* channelData = analysisBuffer.getWritePointer(channel);
            double decibelSumChannel = 0.0;
            int numSamples = analysisBuffer.getNumSamples();
            
            for (int s = 0; s < numSamples; s++)
            {
                float sample = abs(channelData[s]);
                decibelSumChannel += sample;
                
                if (b == numBlocks - 1 && s == lastBlockLength - 1)
                {
                    s = numSamples;
                }
            }
            
            decibelSumChannel /= numSamples;
            decibelSumBlock += decibelSumChannel;
        }
        
        decibelSumBlock /= numChannels;
        decibel += decibelSumBlock;
    }
    
    decibel /= numBlocks;
    decibel = jmax<float>(decibel, 0.00000001);
    
    return 20 * log10(decibel);
}

double SampleAnalyser::analyseSampleLoudnessLUFS(const File &inFile)
{
    return 0.0;
}

void SampleAnalyser::loadAudioFileSource(File const & inFile)
{
    mCurrentAudioFileSource.reset();
    AudioFormatReader* reader = mFormatManager->createReaderFor(inFile);
    mCurrentAudioFileSource = std::make_unique<AudioFormatReaderSource>(reader, true);
}
