/*
 ==============================================================================
 
 SampleAnalyser.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleAnalyser.h"

SampleAnalyser::SampleAnalyser()
:
mForwardFFT(fftOrder),
hannWindow(sizeof(float) * bufferSize, juce::dsp::WindowingFunction<float>::hann)
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
//    loadAudioFileSource(inFile);
//    int numChannels = mCurrentAudioFileSource->getAudioFormatReader()->numChannels;
//    mAnalysisBuffer = AudioBuffer<float>(numChannels, bufferSize);
//    int64 totalNumSamples = mCurrentAudioFileSource->getTotalLength();
//    int numBlocks = int (totalNumSamples / bufferSize) + 1;
//    int lastBlockLength = totalNumSamples % bufferSize;
//    double decibel = 0.0;
//    
//    for (int b = 0; b < numBlocks; b++)
//    {
//        mCurrentAudioFileSource->getAudioFormatReader()->read(&mAnalysisBuffer, 0, bufferSize, b * bufferSize, true, true);
//        double decibelSumBlock = 0.0;
//        
//        for (int channel = 0; channel < numChannels; ++channel)
//        {
//            auto* channelData = mAnalysisBuffer.getWritePointer(channel);
//            double decibelSumChannel = 0.0;
//            int numSamples = mAnalysisBuffer.getNumSamples();
//            
//            for (int s = 0; s < numSamples; s++)
//            {
//                float sample = abs(channelData[s]);
//                decibelSumChannel += sample;
//                
//                if (b == numBlocks - 1 && s == lastBlockLength - 1)
//                {
//                    s = numSamples;
//                }
//            }
//            
//            decibelSumChannel /= numSamples;
//            decibelSumBlock += decibelSumChannel;
//        }
//        
//        decibelSumBlock /= numChannels;
//        decibel += decibelSumBlock;
//    }
//    
//    decibel /= numBlocks;
//    decibel = jmax<float>(decibel, 0.00000001);
//    
//    return 20 * log10(decibel);
    return 0.0;
}

double SampleAnalyser::analyseSampleLoudnessLUFS(const File &inFile)
{
//    loadAudioFileSource(inFile);
//    int numChannels = mCurrentAudioFileSource->getAudioFormatReader()->numChannels;
//    mAnalysisBuffer = AudioBuffer<float>(numChannels, bufferSize);
//    int64 totalNumSamples = mCurrentAudioFileSource->getTotalLength();
//    int numBlocks = int (totalNumSamples * 1.0 / bufferSize) + 1;
//    mEbuLoudnessMeter.prepareToPlay(mCurrentAudioFileSource->getAudioFormatReader()->sampleRate,
//                                   mCurrentAudioFileSource->getAudioFormatReader()->numChannels,
//                                   bufferSize);
//    
//    for (int b = 0; b < numBlocks; b++)
//    {
//        mCurrentAudioFileSource->getAudioFormatReader()->read(&mAnalysisBuffer, 0, bufferSize, b * bufferSize, true, true);
//        mEbuLoudnessMeter.processBlock(mAnalysisBuffer);
//    }
//    
//    return mEbuLoudnessMeter.getIntegratedLoudness();
    return 0.0;
}

int SampleAnalyser::analyseSampleTempo(const File &inFile)
{
    // Calculate spectral novelty function
    // -----------------------------------
    // Retrieve STFT spectrum
    loadAudioFileSource(inFile);
    int numChannels = mCurrentAudioFileSource->getAudioFormatReader()->numChannels;
    int sampleRate = mCurrentAudioFileSource->getAudioFormatReader()->sampleRate;
    mAnalysisBuffer = AudioBuffer<float>(numChannels, bufferSize);
    int fftWindowLength = bufferSize;
    int fftHopLength = bufferSize / 2;
    float compressionFactor = 10.0;
    int totalNumSamples = int (mCurrentAudioFileSource->getTotalLength());
    int numFFTWindows = (totalNumSamples - fftWindowLength) / fftHopLength + 1;
    Array<Array<float>> mSTFTSpectrum;
    
    for (int w = 0; w < numFFTWindows; w++)
    {
        // Read audio for window length
        mCurrentAudioFileSource->getAudioFormatReader()->read(&mAnalysisBuffer, 0, fftWindowLength, w * fftHopLength, true, true);
        mWindowedFFTData.clear();
        mWindowedFFTData.insertArray(0, mAnalysisBuffer.getWritePointer(0), mAnalysisBuffer.getNumSamples());
        
        // Apply window function to buffer
        hannWindow.multiplyWithWindowingTable(mWindowedFFTData.getRawDataPointer(), sizeof(mAnalysisBuffer));
        
        // Perform FFT on buffer
        mForwardFFT.performFrequencyOnlyForwardTransform(mWindowedFFTData.getRawDataPointer());
        
        // Apply logarithmic compression to frequency coefficients
        for (int fc = 0; fc < bufferSize; fc++)
        {
            float frequencyCoefficient = mWindowedFFTData[fc];
            float compressedCoefficient = log(1 + compressionFactor * frequencyCoefficient);
            mWindowedFFTData.set(fc, compressedCoefficient);
        }
        
        // Store FFT vector in STFT spectrum
        mSTFTSpectrum.add(mWindowedFFTData);
    }
    
    // Assemble novelty function
    Array<float> noveltyFunction;
    
    for (int w = 1; w < numFFTWindows; w++)
    {
        float localNovelty = 0.0;
        
        for (int fc = 0; fc < bufferSize; fc++)
        {
            // Calculate discrete derivative
            float currentCoefficient = mSTFTSpectrum[w][fc];
            float previousCoefficient = mSTFTSpectrum[w - 1][fc];
            float localDerivative = currentCoefficient - previousCoefficient;
            
            // Apply half-wave rectification and accumulate bin values for each window
            localNovelty += jmax<float>(localDerivative, 0);
        }
        
        noveltyFunction.add(localNovelty);
    }
    
    // Subtract local average
    int averagingWindowLength = 20;
    
    for (int w = 0; w < numFFTWindows - 1; w++)
    {
        float localAverage = 0.0;
        int currentWindowLength = jmin<int>(averagingWindowLength, numFFTWindows - 1 - w);
        
        for (int m = 0; m < currentWindowLength; m++)
        {
            localAverage += noveltyFunction[m] / currentWindowLength;
        }
        
        // Apply half-wave rectification
        noveltyFunction.set(w, jmax<float>(noveltyFunction[w] - localAverage, 0));
    }
    
    // Normalize novelty function
    float max = *std::max_element(noveltyFunction.getRawDataPointer(), noveltyFunction.getRawDataPointer() + numFFTWindows - 1);
    
    for (int w = 0; w < numFFTWindows - 1; w++)
    {
        noveltyFunction.set(w, noveltyFunction[w] / max);
    }
    
    // Calculate Fourier Tempogram
    // ---------------------------
    // Define set of tempo sinusoids (50 - 300 bpm)
    int lowerBPMLimit = 50;
    int upperBPMLimit = 300;
    int numTempi = upperBPMLimit - lowerBPMLimit;
    
    // Project spectrum onto each tempo sinusoid to calculate tempo coefficient
    int noveltyFunctionSampleRate = sampleRate / fftHopLength;
    int tempogramHopLength = 10;
    int tempogramWindowLength = noveltyFunctionSampleRate * 3; // 3 seconds
    int numTempogramWindows = ((numFFTWindows - tempogramWindowLength) / tempogramHopLength) + 1;
    Array<Array<float>> spectralTempogram;
    hannWindow.fillWindowingTables(sizeof(float) * tempogramWindowLength, juce::dsp::WindowingFunction<float>::hann);
    
    // Padding novelty function to apply a centered windowing function at position t = 0
    int paddingLength = tempogramWindowLength / 2;
    float zeroPadding[paddingLength];
    memset(zeroPadding, 0, sizeof(float) * paddingLength);
    noveltyFunction.insertArray(0, zeroPadding, paddingLength);
    noveltyFunction.addArray(zeroPadding, paddingLength);
    std::complex<float> i(0.0, 1.0);
    
    // Loop over all window positions in the novelty function
    for (int t = 0; t < numTempi; t++)
    {
        float beatsPerSample = ((t + lowerBPMLimit) / 60) / noveltyFunctionSampleRate;
        std::complex<float> exponentialNoveltyFunction[noveltyFunction.size()];
        
        for (int s = 0; s < noveltyFunction.size(); s++)
        {
            exponentialNoveltyFunction[s] = std::exp(-2 * float (M_PI) * i * beatsPerSample * float (s)) * noveltyFunction[s];
        }
        
        for (int w = 0; w < numTempogramWindows; w++)
        {
            int windowStart = w * tempogramHopLength;
            int windowEnd = windowStart + tempogramWindowLength;
            float buffer[tempogramWindowLength];
            
            hannWindow.multiplyWithWindowingTable(buffer, sizeof(float) * tempogramWindowLength);
            
            for (int b = 0; b < tempogramWindowLength; b++)
            {
                spectralTempogram[w].set(t, spectralTempogram[w][t] + buffer[b]);
            }
        }
    }
    
    // Estimate global sample tempo
    // ----------------------------
    // Pick optimal tempo estimation for each position
    // of the tempogram
    float averageTempo = 0.0;
    
    for (int w = 0; w < numTempogramWindows; w++)
    {
        int optimalTempoIndex = 0;
        float optimalTempoEstimation = 0.0;
        
        for (int t = 0; t < numTempi; t++)
        {
            float currentTempoEstimation = spectralTempogram[w][t];
            
            if (currentTempoEstimation > optimalTempoEstimation)
            {
                optimalTempoEstimation = currentTempoEstimation;
                optimalTempoIndex = t;
            }
        }
        
        // Calculate average of all optimal tempo estimations
        averageTempo += (optimalTempoIndex + lowerBPMLimit) / numTempogramWindows;
    }
    
    return 0;
}

void SampleAnalyser::loadAudioFileSource(File const & inFile)
{
    mCurrentAudioFileSource.reset();
    AudioFormatReader* reader = mFormatManager->createReaderFor(inFile);
    mCurrentAudioFileSource = std::make_unique<AudioFormatReaderSource>(reader, true);
}
