/*
 ==============================================================================
 
 SampleAnalyser.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleAnalyser.h"

SampleAnalyser::SampleAnalyser()
:
mForwardFFT(tempoFFTOrder),
hannWindow(sizeof(tempoBufferLength), juce::dsp::WindowingFunction<float>::hann)
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
    
    // Set item properties
    inSampleItem.setLength(analyseSampleLength(inFile));
    inSampleItem.setLoudnessDecibel(0.0);
//    inSampleItem.setLoudnessDecibel(analyseSampleLoudnessDecibel());
    inSampleItem.setLoudnessLUFS(0.0);
//    inSampleItem.setLoudnessLUFS(analyseSampleLoudnessLUFS());
    
    // Don't analyse samples that are less than 4 beats of the lower bpm limit long
//    if (inSampleItem.getLength() < 60.0 / lowerBPMLimit * 4)
//    {
//        inSampleItem.setTempo(0.0);
//    }
//    else
//    {
//        inSampleItem.setTempo(analyseSampleTempo());
//    }
    inSampleItem.setTempo(0);
    
    inSampleItem.setKey(analyseSampleKey());
}

void SampleAnalyser::loadAudioFileSource(File const & inFile)
{
    mCurrentAudioFileSource.reset();
    AudioFormatReader* reader = mFormatManager->createReaderFor(inFile);
    mCurrentAudioFileSource = std::make_unique<AudioFormatReaderSource>(reader, true);
    numChannels = mCurrentAudioFileSource->getAudioFormatReader()->numChannels;
    sampleRate = mCurrentAudioFileSource->getAudioFormatReader()->sampleRate;
    numBlocks = int (totalNumSamples / tempoBufferLength) + 1;
}

double SampleAnalyser::analyseSampleLength(File const & inFile)
{
    double length = mCurrentAudioFileSource->getTotalLength() / mCurrentAudioFileSource->getAudioFormatReader()->sampleRate;
    
    return length;
}

double SampleAnalyser::analyseSampleLoudnessDecibel()
{
    mAnalysisBuffer = AudioBuffer<float>(numChannels, loudnessBufferSize);
    int lastBlockLength = totalNumSamples % loudnessBufferSize;
    double decibel = 0.0;

    for (int b = 0; b < numBlocks; b++)
    {
        mCurrentAudioFileSource->getAudioFormatReader()->read(&mAnalysisBuffer, 0, loudnessBufferSize, b * loudnessBufferSize, true, true);
        double decibelSumBlock = 0.0;

        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto* channelData = mAnalysisBuffer.getWritePointer(channel);
            double decibelSumChannel = 0.0;
            int numSamples = mAnalysisBuffer.getNumSamples();

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

double SampleAnalyser::analyseSampleLoudnessLUFS()
{
    mAnalysisBuffer = AudioBuffer<float>(numChannels, loudnessBufferSize);
    mEbuLoudnessMeter.prepareToPlay(mCurrentAudioFileSource->getAudioFormatReader()->sampleRate,
                                   mCurrentAudioFileSource->getAudioFormatReader()->numChannels,
                                   loudnessBufferSize);

    for (int b = 0; b < numBlocks; b++)
    {
        mCurrentAudioFileSource->getAudioFormatReader()->read(&mAnalysisBuffer, 0, loudnessBufferSize, b * loudnessBufferSize, true, true);
        mEbuLoudnessMeter.processBlock(mAnalysisBuffer);
    }

    return mEbuLoudnessMeter.getIntegratedLoudness();
}

void SampleAnalyser::calculateSTFTSpectrum(int inWindowLength, int inFFTHopLength)
{
    mSTFTSpectrum.clear();
    totalNumSamples = int (mCurrentAudioFileSource->getTotalLength());
    fftWindowLength = jmin<int>(inWindowLength, totalNumSamples);
    numFFTWindows = ((totalNumSamples - fftWindowLength) / inFFTHopLength) + 1;
    hannWindow.fillWindowingTables(sizeof(inWindowLength), juce::dsp::WindowingFunction<float>::hann);
    mAnalysisBuffer = AudioBuffer<float>(numChannels, inWindowLength);
    
    for (int w = 0; w < numFFTWindows; w++)
    {
        // Read audio for window length
        mCurrentAudioFileSource->getAudioFormatReader()->read(&mAnalysisBuffer, 0, fftWindowLength, w * inFFTHopLength, true, true);
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
    Array<float> noveltyFunction;
    
    // Calculate discrete derivative
    for (int w = 1; w < numFFTWindows; w++)
    {
        float localNovelty = 0.0;
        
        for (int fc = 0; fc < tempoBufferLength / 2 + 1; fc++)
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
    noveltyFunctionSampleRate = sampleRate / tempoFFTHopLength;
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
        noveltyFunction.getReference(w) /= max;
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
        localEstimationsForCurrentTempo.resize(numTempogramWindows);
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
        
        // Apply windowing to tempo projection for each position
        for (int w = 0; w < numTempogramWindows; w++)
        {
            int windowStart = w * tempogramHopLength;
            int windowEnd = windowStart + tempogramWindowLength;
            float windowedProjection[tempogramWindowLength];
            std::copy(noveltyFunctionProjection + windowStart, noveltyFunctionProjection + windowEnd, windowedProjection);
            hannWindow.multiplyWithWindowingTable(windowedProjection, tempogramWindowLength);
            
            // Accumulate all windowed values for the current window position
            for (int m = 0; m < tempogramWindowLength; m++)
            {
                localEstimationsForCurrentTempo.getReference(w) += windowedProjection[m];
            }
        }
        
        spectralTempogram.add(localEstimationsForCurrentTempo);
    }
    
    return spectralTempogram;
}

int SampleAnalyser::analyseSampleTempo()
{
    // Calculate STFT spectrum
    calculateSTFTSpectrum(tempoBufferLength, tempoFFTHopLength);

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
    
    // Calculate histogram of optimal local tempo
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
    
    // Find the most prominent tempo in the histogram
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
    
    // return 120;
}

String SampleAnalyser::analyseSampleKey()
{
    // Calculate STFT spectrum
    calculateSTFTSpectrum(keyBufferLength, keyFFTHopLength);
    
    // Calculate logarithmic spectrogram
    Array<Array<float>> logSpectrum;
    
    // Apply logarithmic frequency pooling
    // Loop over all pitch pools
    int numPitches = 128;
    
    for (int p = 0; p < numPitches; p++)
    {
        Array<float> localPitchCoefficients;
        localPitchCoefficients.resize(numFFTWindows);
        float lowerFrequencyLimit = pitchToFrequency(p - 0.5);
        float upperFrequencyLimit = pitchToFrequency(p + 0.5);
        
        // Loop over all frequency coefficients in the spectrum
        for (int fc = 0; fc < keyBufferLength / 2 + 1; fc++)
        {
            float currentFrequency = fc * sampleRate / keyBufferLength;
            
            if (lowerFrequencyLimit <= currentFrequency && currentFrequency <= upperFrequencyLimit)
            {
                // Sum up all coefficients in the current pitch pool for each window
                for (int w = 0; w < numFFTWindows; w++)
                {
                    localPitchCoefficients.getReference(w) += mSTFTSpectrum.getReference(w).getReference(fc);
                }
            }
        }
        
        logSpectrum.add(localPitchCoefficients);
    }
    
    Array<Array<float>> chromagram;
    
    // Calculate chromagram
    for (int c = 0; c < numChroma; c++)
    {
        Array<float> localChromaCoefficients;
        localChromaCoefficients.resize(numFFTWindows);
        
        // Loop over all octaves (skip the 8 topmost pitches for efficiency)
        for (int o = 0; o < 10; o++)
        {
            for (int w = 0; w < numFFTWindows; w++)
            {
                localChromaCoefficients.getReference(w) += logSpectrum.getReference(c + (12 * o)).getReference(w);
            }
        }
        
        chromagram.add(localChromaCoefficients);
    }
    
    // Calculate histogram of most optimal local chroma
    int chromaEstimationsHistogram[numChroma];
    memset(chromaEstimationsHistogram, 0, sizeof(chromaEstimationsHistogram));
    
    for (int w = 0; w < numFFTWindows; w++)
    {
        // Pick optimal chroma estimation for each position of the chromagram
        int optimalChromaIndex = 0;
        float optimalChromaEstimation = 0.0;
        
        for (int c = 0; c < numChroma; c++)
        {
            float currentChromaEstimation = chromagram.getReference(c).getReference(w);
            
            if (currentChromaEstimation > optimalChromaEstimation)
            {
                optimalChromaEstimation = currentChromaEstimation;
                optimalChromaIndex = c;
            }
        }
        
        // Assemble histogram of all chroma estimations
        chromaEstimationsHistogram[optimalChromaIndex] += 1;
    }
    
    // Find the most prominent chroma in the chroma
    int bestChromaIndex = 0;
    int bestChromaBinHeight = 0;
    
    for (int c = 0; c < numChroma; c++)
    {
        float currentEstimation = chromaEstimationsHistogram[c];
        
        if (currentEstimation > bestChromaBinHeight)
        {
            bestChromaIndex = c;
            bestChromaBinHeight = currentEstimation;
        }
    }
    
    return CHROMA_INDEX_TO_KEY[bestChromaIndex];
}

float SampleAnalyser::pitchToFrequency(float pitchIndex)
{
    return pow(2, (pitchIndex - 69) / numChroma) * 440;
}
