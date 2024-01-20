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
hannWindow(sizeof(bufferSize), juce::dsp::WindowingFunction<float>::hann)
{
    mFormatManager = std::make_unique<AudioFormatManager>();
    mFormatManager->registerBasicFormats();
}

SampleAnalyser::~SampleAnalyser()
{
    
}

void SampleAnalyser::analyseSample(SampleItem& inSampleItem, File const & inFile)
{
    // Load audio file
    loadAudioFileSource(inFile);
    numChannels = mCurrentAudioFileSource->getAudioFormatReader()->numChannels;
    sampleRate = mCurrentAudioFileSource->getAudioFormatReader()->sampleRate;
    numBlocks = int (totalNumSamples / bufferSize) + 1;
    mAnalysisBuffer = AudioBuffer<float>(numChannels, bufferSize);
    
    // Calculate STFT spectrum
    calculateSTFTSpectrum();
    
    // Set item properties
    inSampleItem.setLength(analyseSampleLength(inFile));
    inSampleItem.setLoudnessDecibel(analyseSampleLoudnessDecibel(inFile));
    inSampleItem.setLoudnessLUFS(analyseSampleLoudnessLUFS(inFile));
    
    if (inSampleItem.getLength() < 4.0)
    {
        inSampleItem.setTempo(0.0);
    }
    else
    {
        inSampleItem.setTempo(analyseSampleTempo(inFile));
    }
}

void SampleAnalyser::loadAudioFileSource(File const & inFile)
{
    mCurrentAudioFileSource.reset();
    AudioFormatReader* reader = mFormatManager->createReaderFor(inFile);
    mCurrentAudioFileSource = std::make_unique<AudioFormatReaderSource>(reader, true);
}

double SampleAnalyser::analyseSampleLength(File const & inFile)
{
    double length = mCurrentAudioFileSource->getTotalLength() / mCurrentAudioFileSource->getAudioFormatReader()->sampleRate;
    return length;
}

double SampleAnalyser::analyseSampleLoudnessDecibel(const File &inFile)
{
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

void SampleAnalyser::calculateSTFTSpectrum()
{
    mSTFTSpectrum.clear();
    totalNumSamples = int (mCurrentAudioFileSource->getTotalLength());
    fftWindowLength = jmin<int>(bufferSize, totalNumSamples);
    numFFTWindows = ((totalNumSamples - fftWindowLength) / fftHopLength) + 1;
    hannWindow.fillWindowingTables(sizeof(bufferSize), juce::dsp::WindowingFunction<float>::hann);
    
    for (int w = 0; w < numFFTWindows; w++)
    {
        // Read audio for window length
        mCurrentAudioFileSource->getAudioFormatReader()->read(&mAnalysisBuffer, 0, fftWindowLength, w * fftHopLength, true, true);
        mWindowedFFTData.clear();
        
        // Create mono signal
        for (int s = 0; s < mAnalysisBuffer.getNumSamples(); s++)
        {
            float summedChannelMagnitude = 0.0;
            
            for (int ch = 0; ch < numChannels; ch++)
            {
                summedChannelMagnitude += mAnalysisBuffer.getSample(ch, s);
            }
            
            mWindowedFFTData.set(s, summedChannelMagnitude);
        }
        
        
        // Apply window function to buffer
        hannWindow.multiplyWithWindowingTable(mWindowedFFTData.getRawDataPointer(), sizeof(mAnalysisBuffer));
        
        // Perform FFT on buffer
        mForwardFFT.performFrequencyOnlyForwardTransform(mWindowedFFTData.getRawDataPointer());
        
        // Apply logarithmic compression to frequency coefficients
        if (compressionFactor > 0)
        {
            for (int fc = 0; fc < mWindowedFFTData.size() / 2 + 1; fc++)
            {
                float frequencyCoefficient = mWindowedFFTData[fc];
                float compressedCoefficient = log(1 + compressionFactor * frequencyCoefficient);
                mWindowedFFTData.set(fc, compressedCoefficient);
            }
        }
        
        // Store FFT vector in STFT spectrum
        mSTFTSpectrum.add(mWindowedFFTData);
    }
}

Array<float> SampleAnalyser::calculateSpectralNoveltyFunction()
{
    Array<float> noveltyFunction = Array<float>(numFFTWindows - 1);
    
    // Calculate discrete derivative
    for (int w = 1; w < numFFTWindows; w++)
    {
        float localNovelty = 0.0;
        
        for (int fc = 0; fc < bufferSize / 2 + 1; fc++)
        {
            float currentCoefficient = mSTFTSpectrum.getReference(w).getReference(fc);
            float previousCoefficient = mSTFTSpectrum.getReference(w - 1).getReference(fc);
            float localDerivative = currentCoefficient - previousCoefficient;
            
            // Apply half-wave rectification and accumulate bin values for each window
            localNovelty += jmax<float>(localDerivative, 0);
        }
        
        noveltyFunction.set(w - 1, localNovelty);
    }
    
    // Subtract local average
    noveltyFunctionSampleRate = sampleRate / fftHopLength;
    int noveltyAveragingWindowLength = noveltyFunctionSampleRate * noveltyAveragingWindowLengthInSeconds;
    
    for (int w = 0; w < numFFTWindows - 1; w++)
    {
        float localAverage = 0.0;
        int currentWindowStart = jmax<int>(w - noveltyAveragingWindowLength, 0);
        int currentWindowEnd = jmin<int>(w + noveltyAveragingWindowLength + 1, numFFTWindows - 1);
        int currentWindowLength = currentWindowEnd - currentWindowStart;
        
        for (int m = 0; m < currentWindowLength; m++)
        {
            localAverage += noveltyFunction.getReference(m) / currentWindowLength;
        }
        
        // Apply half-wave rectification
        noveltyFunction.set(w, jmax<float>(noveltyFunction.getReference(w) - localAverage, 0));
    }
    
    // Normalize novelty function
    float max = *std::max_element(noveltyFunction.getRawDataPointer(), noveltyFunction.getRawDataPointer() + numFFTWindows - 1);
    
    for (int w = 0; w < numFFTWindows - 1; w++)
    {
        noveltyFunction.set(w, noveltyFunction.getReference(w) / max);
    }
    
    return noveltyFunction;
}

Array<Array<float>> SampleAnalyser::calculateFourierTempogram(Array<float>& noveltyFunction,
                                                              int noveltyFunctionSampleRate,
                                                              int numTempogramWindows,
                                                              int tempogramWindowLength)
{
    Array<Array<float>> spectralTempogram;
    hannWindow.fillWindowingTables(sizeof(tempogramWindowLength), juce::dsp::WindowingFunction<float>::hann);
    std::complex<float> i(0.0, 1.0);
    
    // Project novelty function onto each tempo sinusoid
    for (int t = 0; t < numTempi; t++)
    {
        Array<float> localEstimationsForCurrentTempo;
        float beatsPerSample = ((t + lowerBPMLimit) * 1.0 / 60) / noveltyFunctionSampleRate;
        float noveltyFunctionProjection[noveltyFunction.size()];
        
        for (int s = 0; s < noveltyFunction.size(); s++)
        {
            noveltyFunctionProjection[s] = (std::exp(-2
                                                     * float (M_PI)
                                                     * i
                                                     * beatsPerSample
                                                     * float (s))
                                            * noveltyFunction.getReference(s)).real();
        }
        
        for (int w = 0; w < numTempogramWindows; w++)
        {
            localEstimationsForCurrentTempo.add(0.0);
            int windowStart = w * tempogramHopLength;
            int windowEnd = windowStart + tempogramWindowLength;
            float windowedProjection[tempogramWindowLength];
            std::copy(noveltyFunctionProjection + windowStart, noveltyFunctionProjection + windowEnd, windowedProjection);
            hannWindow.multiplyWithWindowingTable(windowedProjection, tempogramWindowLength);
            
            for (int m = 0; m < tempogramWindowLength; m++)
            {
                localEstimationsForCurrentTempo.getReference(w) += windowedProjection[m];
            }
        }
        
        spectralTempogram.add(localEstimationsForCurrentTempo);
    }
    
    return spectralTempogram;
}

int SampleAnalyser::analyseSampleTempo(File const & inFile)
{
    // Calculate spectral novelty function
    Array<float> noveltyFunction = calculateSpectralNoveltyFunction();
    
    // Padding novelty function to apply a centered windowing function at position t = 0
    int tempogramWindowLength = jmin(noveltyFunctionSampleRate * tempogramWindowLengthInSeconds, noveltyFunction.size());
    int paddingLength = tempogramWindowLength / 2;
    float zeroPadding[paddingLength];
    memset(zeroPadding, 0, sizeof(float) * paddingLength);
    noveltyFunction.insertArray(0, zeroPadding, paddingLength);
    noveltyFunction.addArray(zeroPadding, paddingLength);
    int numTempogramWindows = ((noveltyFunction.size() - tempogramWindowLength) / tempogramHopLength) + 1;
    
    // Calculate Fourier Tempogram
    Array<Array<float>> spectralTempogram = calculateFourierTempogram(noveltyFunction,
                                                                      noveltyFunctionSampleRate,
                                                                      numTempogramWindows,
                                                                      tempogramWindowLength);
    
    // Estimate global sample tempo
    int tempoEstimationsHistogram[numTempi];
    memset(tempoEstimationsHistogram, 0, sizeof(tempoEstimationsHistogram));
    
    for (int w = 0; w < numTempogramWindows; w++)
    {
        // Pick optimal tempo estimation for each position of the tempogram
        int optimalTempoIndex = 0;
        float optimalTempoEstimation = 0.0;
        
        for (int t = 0; t < numTempi; t++)
        {
            float currentTempoEstimation = spectralTempogram.getReference(t).getReference(w);
            
            if (currentTempoEstimation > optimalTempoEstimation)
            {
                optimalTempoEstimation = currentTempoEstimation;
                optimalTempoIndex = t;
            }
        }
        
        // Assemble histogram of all tempo estimations
        tempoEstimationsHistogram[optimalTempoIndex] += 1;
    }
    
    int bestTempoIndex = 0;
    int bestTempoBinHeight = 0;
    
    for (int t = ignoreTopAndBottomTempi; t < numTempi - ignoreTopAndBottomTempi; t++)
    {
        float currentEstimation = tempoEstimationsHistogram[t];
        
        if (currentEstimation > bestTempoBinHeight)
        {
            bestTempoIndex = t;
            bestTempoBinHeight = currentEstimation;
        }
    }
    
    int bestTempoEstimation = bestTempoIndex + lowerBPMLimit;
    return bestTempoEstimation;
}
