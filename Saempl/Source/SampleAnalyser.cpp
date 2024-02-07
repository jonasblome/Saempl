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
mWindowFunction(tempoWindowLength + 1, juce::dsp::WindowingFunction<float>::hann)
{
    mFormatManager = std::make_unique<AudioFormatManager>();
    mFormatManager->registerBasicFormats();
}

SampleAnalyser::~SampleAnalyser()
{
    
}

void SampleAnalyser::analyseSample(SampleItem& inSampleItem, File const & inFile, bool forceAnalysis)
{
    std::vector<float> featureVector = std::vector<float>(NUM_SPECTRAL_BANDS + NUM_CHROMA + NUM_FEATURES);
    
    // Load audio file
    // DBG(inSampleItem.getTitle());
    loadAudioFileSource(inFile);
    
    // Set sample length
    float length = totalNumSamples * 1.0 / sampleRate;
    inSampleItem.setLength(length);
    featureVector[0] = length / 100;
    
    // Set sample loudness and loudness range
    analyseSampleLoudness();
    inSampleItem.setLoudnessDecibel(decibel);
    inSampleItem.setLoudnessLUFS(integratedLUFS);
    featureVector[1] = integratedLUFS / (-100);
    featureVector[2] = lufsRangeStart / (-100);
    featureVector[3] = lufsRangeEnd / (-100);
    
    // Set zero crossing rate
    featureVector[4] = zeroCrossingRate;
    
    // Maximum length for initial analysis is 1 minute
    if (length <= 60 || forceAnalysis)
    {
        // Set sample tempo
        float tempo = analyseSampleTempo();
        inSampleItem.setTempo(tempo);
        featureVector[5] = (tempo - lowerBPMLimit) / (upperBPMLimit - lowerBPMLimit);
        
        // Set sample key
        int key = analyseSampleKey();
        inSampleItem.setKey(key);
        featureVector[6] = key * 1.0 / 12;
        
        // Set spectral centroid
        featureVector[7] = spectralCentroid / 20000;
        
        // Set spectral spread
        featureVector[8] = spectralSpread / 100;
        
        // Set spectral roll off
        featureVector[9] = spectralRollOffBandIndex * 1.0 / NUM_SPECTRAL_BANDS;
        
        // Set spectral flux
        featureVector[10] = spectralFlux / 10;
        
        // Set chroma flux
        featureVector[11] = chromaFlux / 1000;
        
        // Set spectral distribution coefficients
        for (int b = 0; b < NUM_SPECTRAL_BANDS; b++)
        {
            featureVector[NUM_FEATURES + b] = mSpectralDistribution[b];
        }
        
        // Set chroma distribution coefficients
        for (int c = 0; c < NUM_CHROMA; c++)
        {
            featureVector[NUM_FEATURES + NUM_SPECTRAL_BANDS + c] = mChromaDistribution[c];
        }
    }
    
    // Set sample item's feature vector
    inSampleItem.setFeatureVector(featureVector);
}

void SampleAnalyser::loadAudioFileSource(File const & inFile)
{
    mCurrentAudioFileSource.reset();
    AudioFormatReader* reader = mFormatManager->createReaderFor(inFile);
    mCurrentAudioFileSource = std::make_unique<AudioFormatReaderSource>(reader, true);
    numChannels = mCurrentAudioFileSource->getAudioFormatReader()->numChannels;
    sampleRate = mCurrentAudioFileSource->getAudioFormatReader()->sampleRate;
    totalNumSamples = int (mCurrentAudioFileSource->getTotalLength());
    numBlocks = int (totalNumSamples / loudnessBufferSize) + 1;
}

void SampleAnalyser::analyseSampleLoudness()
{
    // Reset parameters
    mAnalysisBuffer = AudioBuffer<float>(numChannels, loudnessBufferSize);
    decibel = 0.0;
    integratedLUFS = 0.0;
    numZeroCrossings = 0;
    mEbuLoudnessMeter.prepareToPlay(sampleRate,
                                    numChannels,
                                    loudnessBufferSize);
    
    for (int b = 0; b < numBlocks; b++)
    {
        // Read in block of audio
        mCurrentAudioFileSource->getAudioFormatReader()->read(&mAnalysisBuffer,
                                                              0,
                                                              loudnessBufferSize,
                                                              b * loudnessBufferSize,
                                                              true,
                                                              true);
        
        // Calculate LUFS and Decibels for block
        mEbuLoudnessMeter.processBlock(mAnalysisBuffer, numZeroCrossings, decibel);
    }
    
    // Calculate Decibel
    decibel /= numBlocks;
    decibel = jmax<float>(decibel, 0.00000001);
    decibel = 20 * log10(decibel);
    
    // Retrieve LUFS values
    integratedLUFS = mEbuLoudnessMeter.getIntegratedLoudness();
    lufsRangeStart = mEbuLoudnessMeter.getLoudnessRangeStart();
    lufsRangeEnd = mEbuLoudnessMeter.getLoudnessRangeEnd();
    
    // Calculate zero crossing rate
    zeroCrossingRate = (numZeroCrossings * 1.0 / numChannels) * 1.0 / totalNumSamples;
}

void SampleAnalyser::calculateSTFTSpectrum(int inFFTOrder,
                                           int inWindowLength,
                                           int inFFTHopLength,
                                           float inCompressionFactor)
{
    mSTFTSpectrum.clear();
    mForwardFFT = dsp::FFT(inFFTOrder);
    numFFTWindows = ((jmax<int>(0, totalNumSamples - inWindowLength)) / inFFTHopLength) + 1;
    mWindowFunction.fillWindowingTables(inWindowLength + 1, juce::dsp::WindowingFunction<float>::hann);
    mAnalysisBuffer = AudioBuffer<float>(numChannels, inWindowLength);
    mWindowedFFTData.clear();
    mWindowedFFTData.resize(inWindowLength * 2);
    
    // Loop over all windows
    for (int w = 0; w < numFFTWindows; w++)
    {
        // Read audio for window length
        mCurrentAudioFileSource->getAudioFormatReader()->read(&mAnalysisBuffer,
                                                              0,
                                                              inWindowLength,
                                                              w * inFFTHopLength,
                                                              true,
                                                              true);
        
        // Create mono signal and write into FFT data array
        for (int s = 0; s < inWindowLength; s++)
        {
            float summedChannelMagnitude = 0.0;
            
            for (int ch = 0; ch < numChannels; ch++)
            {
                summedChannelMagnitude += mAnalysisBuffer.getSample(ch, s);
            }
            
            mWindowedFFTData.set(s, summedChannelMagnitude);
        }
        
        // Apply window function to buffer
        mWindowFunction.multiplyWithWindowingTable(mWindowedFFTData.getRawDataPointer(), inWindowLength);
        
        // Perform FFT on buffer
        mForwardFFT.performFrequencyOnlyForwardTransform(mWindowedFFTData.getRawDataPointer(), true);
        
        // Apply logarithmic compression to frequency coefficients
        if (inCompressionFactor > 0.0)
        {
            for (int fc = 0; fc < (inWindowLength / 2) + 1; fc++)
            {
                float frequencyCoefficient = mWindowedFFTData.getReference(fc);
                float compressedCoefficient = log(1 + tempoCompressionFactor * frequencyCoefficient);
                mWindowedFFTData.set(fc, compressedCoefficient);
            }
        }
        
        // Store FFT vector in STFT spectrum
        mSTFTSpectrum.add(mWindowedFFTData);
    }
}

Array<float> SampleAnalyser::calculateNoveltyFunction()
{
    Array<float> noveltyFunction;
    int numCoefficients = (tempoWindowLength / 2) + 1;
    float coefficientSum = 0.0;
    spectralFlux = 0.0;
    spectralCentroid = 0.0;
    
    // Calculate discrete derivative
    for (int w = 1; w < numFFTWindows; w++)
    {
        float localNovelty = 0.0;
        
        for (int fc = 0; fc < numCoefficients; fc++)
        {
            // Calculate derivative
            float currentCoefficient = mSTFTSpectrum.getReference(w).getReference(fc);
            float previousCoefficient = mSTFTSpectrum.getReference(w - 1).getReference(fc);
            float localDerivative = currentCoefficient - previousCoefficient;
            coefficientSum += currentCoefficient;
            
            // Calculate spectral flux
            spectralFlux += abs(localDerivative);
            
            // Calculate spectral centroid
            spectralCentroid += fc * currentCoefficient;
            
            // Apply half-wave rectification and accumulate bin values for each window
            localNovelty += jmax<float>(localDerivative, 0);
        }
        
        noveltyFunction.set(w - 1, localNovelty);
    }
    
    // Calculate spectral flux
    spectralFlux = spectralFlux / coefficientSum;
    
    // Calculate spectral centroid
    spectralCentroid = spectralCentroid / coefficientSum;
    spectralCentroid = (spectralCentroid * sampleRate) / tempoWindowLength;
    
    return noveltyFunction;
}

void SampleAnalyser::noveltyFunctionSubtractAverage(Array<float> &noveltyFunction)
{
    noveltyFunctionSampleRate = sampleRate / tempoFFTHopLength;
    int noveltyAveragingWindowLength = noveltyFunctionSampleRate * noveltyAveragingWindowLengthInSeconds;
    
    for (int w = 0; w < numFFTWindows - 1; w++)
    {
        float localAverage = 0.0;
        int currentWindowStart = jmax<int>(w - noveltyAveragingWindowLength, 0);
        int currentWindowEnd = jmin<int>(w + noveltyAveragingWindowLength + 1, numFFTWindows - 1);
        int currentWindowLength = currentWindowEnd - currentWindowStart;
        
        for (int m = currentWindowStart; m < currentWindowEnd; m++)
        {
            localAverage += noveltyFunction.getReference(m) / currentWindowLength;
        }
        
        // Apply half-wave rectification
        noveltyFunction.set(w, jmax<float>(noveltyFunction.getReference(w) - localAverage, 0));
    }
}

Array<Array<float>> SampleAnalyser::calculateTempogram(Array<float> & noveltyFunction, int & numTempogramWindows)
{
    Array<Array<float>> spectralTempogram;
    int tempogramWindowLength = jmin(noveltyFunctionSampleRate * tempogramWindowLengthInSeconds, noveltyFunction.size());
    int paddingLength = tempogramWindowLength / 2;
    float zeroPadding[paddingLength];
    memset(zeroPadding, 0, sizeof(float) * paddingLength);
    noveltyFunction.insertArray(0, zeroPadding, paddingLength);
    noveltyFunction.addArray(zeroPadding, paddingLength);
    numTempogramWindows = ((noveltyFunction.size() - tempogramWindowLength) / tempogramHopLength) + 1;
    
    // Calculate Fourier Tempogram
    mWindowFunction.fillWindowingTables(tempogramWindowLength + 1, juce::dsp::WindowingFunction<float>::hann);
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
            mWindowFunction.multiplyWithWindowingTable(windowedProjection, tempogramWindowLength);
            
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

Array<int> SampleAnalyser::calculateTempoHistogram(int numTempogramWindows, Array<Array<float>> & spectralTempogram)
{
    Array<int> tempoEstimationsHistogram;
    tempoEstimationsHistogram.resize(numTempi);
    
    for (int w = 0; w < numTempogramWindows; w++)
    {
        // Pick optimal tempo estimation for each position of the tempogram
        int optimalTempoIndex = 0;
        float optimalTempoBinHeight = 0.0;
        float localAverageBinHeight = 0.0;
        
        for (int t = 0; t < numTempi; t++)
        {
            float currentTempoBinHeight = spectralTempogram.getReference(t).getReference(w);
            localAverageBinHeight += currentTempoBinHeight;
            
            if (currentTempoBinHeight > optimalTempoBinHeight)
            {
                optimalTempoBinHeight = currentTempoBinHeight;
                optimalTempoIndex = t;
            }
        }
        
        // Assemble histogram of all tempo estimations
        localAverageBinHeight /= numTempi;
        
        // Check if optimal is above local average
        if (optimalTempoBinHeight > localAverageBinHeight * tempoAverageBinHeightThresholdFactor)
        {
            tempoEstimationsHistogram.getReference(optimalTempoIndex) += 1;
        }
    }
    
    return tempoEstimationsHistogram;
}

int SampleAnalyser::analyseSampleTempo()
{
    // Calculate STFT spectrum
    calculateSTFTSpectrum(tempoFFTOrder, tempoWindowLength, tempoFFTHopLength, tempoCompressionFactor);
    
    if (mSTFTSpectrum.size() < 2)
    {
        return 0.0;
    }
    
    // Calculate spectral novelty function
    Array<float> noveltyFunction = calculateNoveltyFunction();
    
    // Subtract local average
    noveltyFunctionSubtractAverage(noveltyFunction);
    
    // Normalize novelty function
    float max = *std::max_element(noveltyFunction.getRawDataPointer(),
                                  noveltyFunction.getRawDataPointer()
                                  + numFFTWindows - 1);
    
    for (int w = 0; w < numFFTWindows - 1; w++)
    {
        noveltyFunction.getReference(w) /= max;
    }
    
    // Padding novelty function to apply a centered windowing function at position t = 0
    int numTempogramWindows;
    Array<Array<float>> spectralTempogram = calculateTempogram(noveltyFunction, numTempogramWindows);
    
    // Calculate histogram of optimal local tempo
    Array<int> tempoEstimationsHistogram = calculateTempoHistogram(numTempogramWindows, spectralTempogram);
    
    // Find the most prominent tempo in the histogram
    int bestTempoIndex = (int) std::distance(tempoEstimationsHistogram.getRawDataPointer(),
                                   std::max_element(tempoEstimationsHistogram.getRawDataPointer()
                                                    + ignoreTopAndBottomTempi,
                                                    tempoEstimationsHistogram.getRawDataPointer()
                                                    + numTempi
                                                    - ignoreTopAndBottomTempi));
    
    int bestTempoEstimation = bestTempoIndex + lowerBPMLimit;
    
    return bestTempoEstimation;
}

Array<Array<float>> SampleAnalyser::calculateLogSpectrogram(float& coefficientSum)
{
    // Apply logarithmic frequency pooling
    Array<Array<float>> logSpectrogram;
    int numPitches = 155;
    int numPitchesPerBand = numPitches / NUM_SPECTRAL_BANDS;
    
    // Loop over all pitch pools
    for (int p = 0; p < numPitches; p++)
    {
        float lowerFrequencyLimit = pitchToFrequency(p - 0.5);
        float upperFrequencyLimit = pitchToFrequency(p + 0.5);
        
        Array<float> localPitchCoefficients;
        localPitchCoefficients.resize(numFFTWindows);
        
        // Loop over all frequency coefficients in the pool
        float secondsPerWindow = (keyWindowLength * 1.0) / sampleRate;
        int lowestCoefficientIndexForCurrentPool = (lowerFrequencyLimit * secondsPerWindow) + 1;
        int highestCoefficientIndexForCurrentPool = jmin<int>((keyWindowLength / 2) + 1,
                                                              (upperFrequencyLimit * secondsPerWindow) + 1);
        
        for (int fc = lowestCoefficientIndexForCurrentPool; fc < highestCoefficientIndexForCurrentPool; fc++)
        {
            // For each window, sum up all coefficients in the current pitch pool
            for (int w = 0; w < numFFTWindows; w++)
            {
                float currentCoefficient = mSTFTSpectrum.getReference(w).getReference(fc);
                localPitchCoefficients.getReference(w) += currentCoefficient;
                
                // Calculate spectral distribution
                mSpectralDistribution[p / numPitchesPerBand] += currentCoefficient;
                
                // Calculate spectral spread
                float currentFrequency = (fc * sampleRate) * 1.0 / keyWindowLength;
                float meanError = currentFrequency - spectralCentroid;
                spectralSpread += (meanError * meanError) * currentCoefficient;
                coefficientSum += currentCoefficient;
            }
        }
        
        logSpectrogram.add(localPitchCoefficients);
        
        // Don't go higher than the frequency limit
        if (highestCoefficientIndexForCurrentPool == keyWindowLength / 2 + 1)
        {
            break;
        }
    }
    
    // Calculate spectral spread
    spectralSpread /= coefficientSum;
    spectralSpread /= spectralCentroid;
    spectralSpread = sqrt(spectralSpread);
    
    return logSpectrogram;
}

void SampleAnalyser::calculateChromaDistribution(Array<Array<float>> &logSpectrogram) {
    std::memset(mChromaDistribution, 0, sizeof(mChromaDistribution));
    chromaFlux = 0.0;
    
    // Loop over all pitches
    for (int p = 0; p < logSpectrogram.size(); p++)
    {
        for (int w = 0; w < numFFTWindows; w++)
        {
            float currentCoefficient = logSpectrogram.getReference(p).getReference(w);
            mChromaDistribution[p % NUM_CHROMA] += currentCoefficient;
            
            if (w >= 1)
            {
                float previousCoefficient = logSpectrogram.getReference(p).getReference(w - 1);
                chromaFlux += abs(currentCoefficient - previousCoefficient);
            }
        }
    }
    
    chromaFlux = (chromaFlux / numFFTWindows) / logSpectrogram.size();
    
    // Normalize chroma distribution
    float maxChroma = *std::max_element(mChromaDistribution,
                                        mChromaDistribution
                                        + NUM_CHROMA);
    
    for (int c = 0; c < NUM_CHROMA; c++)
    {
        mChromaDistribution[c] /= maxChroma;
    }
}

Array<float> SampleAnalyser::calculateKeyChromaCorrelations(float & averageCorrelation, int & numKeys)
{
    Array<float> keyChromaCorrelations;
    keyChromaCorrelations.resize(numKeys);
    
    averageCorrelation = 0.0;
    
    for (int k = 0; k < numKeys; k++)
    {
        Array<int> const & keyPattern = KEY_PATTERNS.getReference(k);
        
        for (int c = 0; c < NUM_CHROMA; c++)
        {
            if (keyPattern.contains(c))
            {
                keyChromaCorrelations.getReference(k) += mChromaDistribution[c];
            }
        }
        
        averageCorrelation += keyChromaCorrelations[k];
    }
    
    return keyChromaCorrelations;
}

int SampleAnalyser::analyseSampleKey()
{
    // Calculate STFT spectrum
    calculateSTFTSpectrum(keyFFTOrder, keyWindowLength, keyFFTHopLength, keyCompressionFactor);
    std::memset(mSpectralDistribution, 0, sizeof(mSpectralDistribution));
    
    // Calculate logarithmic spectrogram
    spectralSpread = 0.0;
    float coefficientSum = 0.0;
    Array<Array<float>> logSpectrogram = calculateLogSpectrogram(coefficientSum);
    
    // Normalize spectral distribution
    float maxSpectral = *std::max_element(mSpectralDistribution,
                                          mSpectralDistribution
                                          + NUM_SPECTRAL_BANDS);
    float summedBands = 0.0;
    
    for (int b = 0; b < NUM_SPECTRAL_BANDS; b++)
    {
        mSpectralDistribution[b] /= maxSpectral;
        summedBands += mSpectralDistribution[b];
    }
    
    // Calculate spectral roll off band index
    float energyPercentage = 0.0;
    int b = 0;
    
    while (energyPercentage < 0.9 * summedBands)
    {
        energyPercentage += mSpectralDistribution[b];
        b++;
    }
    
    spectralRollOffBandIndex = b - 1;
    
    // Calculate chroma distribution
    calculateChromaDistribution(logSpectrogram);
    
    // Correlate the key patterns and the chroma distribution
    int numKeys = KEY_PATTERNS.size();
    float averageCorrelation;
    Array<float> keyHistogramCorrelations = calculateKeyChromaCorrelations(averageCorrelation, numKeys);
    
    // Find key with the highest histogram correlation
    averageCorrelation /= numKeys;
    int optimalKeyIndex = NO_KEY_INDEX;
    float* maxCorrelation = std::max_element(keyHistogramCorrelations.getRawDataPointer(),
                                             keyHistogramCorrelations.getRawDataPointer() + numKeys);
    
    if (*maxCorrelation > keyHistogramCorrelationThresholdFactor * averageCorrelation)
    {
        optimalKeyIndex = (int) std::distance(keyHistogramCorrelations.getRawDataPointer(), maxCorrelation);
    }
    
    return optimalKeyIndex;
}

float SampleAnalyser::pitchToFrequency(float pitchIndex, int referenceIndex, float referenceFrequency)
{
    return pow(2, (pitchIndex - referenceIndex) / NUM_CHROMA) * referenceFrequency;
}
