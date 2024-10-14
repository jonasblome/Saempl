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

void SampleAnalyser::analyseSample(SampleItem* inSampleItem, bool forceAnalysis)
{
    // Load audio file
    loadAudioFileSource(inSampleItem->getFilePath());
    
    if (sampleRate == 0 || numChannels == 0 || totalNumSamples == 0 || numBlocks == 0)
    {
        return;
    }
    
    // Set sample length
    float length = totalNumSamples * 1.0 / sampleRate;
    inSampleItem->setLength(length);
    
    // Set sample loudness and loudness range
    analyseSampleLoudness();
    inSampleItem->setLoudnessDecibel(decibel);
    inSampleItem->setLoudnessLUFS(integratedLUFS);
    inSampleItem->setLoudnessLUFSStart(lufsRangeStart + 300 / (3 + 300));
    inSampleItem->setLoudnessLUFSEnd(lufsRangeEnd + 300 / (3 + 300));
    
    // Set zero crossing rate
    inSampleItem->setZeroCrossingRate(zeroCrossingRate);
    
    // Maximum length for initial analysis is 1 minute
    if (length <= 60 || forceAnalysis)
    {
        // Set properties
        float tempo = analyseSampleTempo();
        
        if (length >= 60.0f / upperBPMLimitExpanded * 5)
        {
            inSampleItem->setTempo(tempo);
        }
        
        int key = analyseSampleKey();
        inSampleItem->setKey(key);
        inSampleItem->setSpectralCentroid(spectralCentroid / 20000);
        inSampleItem->setSpectralSpread(spectralSpread / 100);
        inSampleItem->setSpectralRolloff(spectralRollOffBandIndex * 1.0 / NUM_SPECTRAL_BANDS);
        inSampleItem->setSpectralFlux(spectralFlux);
        inSampleItem->setChromaFlux(chromaFlux / 1000);
        inSampleItem->setSpectralDistribution(mSpectralDistribution);
        inSampleItem->setChromaDistribution(mChromaDistribution);
    }
    else
    {
        inSampleItem->setKey(SAMPLE_TOO_LONG_INDEX);
    }
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
    mForwardFFT = dsp::FFT(inFFTOrder);
    numFFTWindows = ((jmax<int>(0, totalNumSamples - inWindowLength)) / inFFTHopLength) + 1;
    mSTFTSpectrum.clear();
    mSTFTSpectrum.resize(numFFTWindows);
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
        
        // Create mono signal and write into FFT data vector
        for (int s = 0; s < inWindowLength; s++)
        {
            float summedChannelMagnitude = 0.0;
            
            for (int ch = 0; ch < numChannels; ch++)
            {
                summedChannelMagnitude += mAnalysisBuffer.getSample(ch, s);
            }
            
            mWindowedFFTData[s] = summedChannelMagnitude;
        }
        
        // Apply window function to buffer
        mWindowFunction.multiplyWithWindowingTable(&*mWindowedFFTData.begin(), inWindowLength);
        
        // Perform FFT on buffer
        mForwardFFT.performFrequencyOnlyForwardTransform(&*mWindowedFFTData.begin(), true);
        
        // Apply logarithmic compression to frequency coefficients
        if (inCompressionFactor > 0.0)
        {
            for (int fc = 0; fc < (inWindowLength / 2) + 1; fc++)
            {
                float frequencyCoefficient = mWindowedFFTData[fc];
                float compressedCoefficient = log(1 + tempoCompressionFactor * frequencyCoefficient);
                mWindowedFFTData[fc] = compressedCoefficient;
            }
        }
        
        // Store FFT vector in STFT spectrum
        mSTFTSpectrum[w] = mWindowedFFTData;
    }
}

std::vector<float> SampleAnalyser::calculateNoveltyFunction()
{
    std::vector<float> noveltyFunction;
    noveltyFunction.resize(numFFTWindows);
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
            float currentCoefficient = mSTFTSpectrum[w][fc];
            float previousCoefficient = mSTFTSpectrum[w - 1][fc];
            float localDerivative = currentCoefficient - previousCoefficient;
            coefficientSum += currentCoefficient;
            
            // Calculate spectral flux
            spectralFlux += abs(localDerivative);
            
            // Calculate spectral centroid
            spectralCentroid += fc * currentCoefficient;
            
            // Apply half-wave rectification and accumulate bin values for each window
            localNovelty += jmax<float>(localDerivative, 0);
        }
        
        noveltyFunction[w - 1] = localNovelty;
    }
    
    // Calculate spectral flux and spectral centroid
    if (coefficientSum != 0)
    {
        spectralFlux = spectralFlux / coefficientSum;
        spectralCentroid = spectralCentroid / coefficientSum;
    }
    
    spectralCentroid = (spectralCentroid * sampleRate) / tempoWindowLength;
    
    return noveltyFunction;
}

void SampleAnalyser::noveltyFunctionSubtractAverage(std::vector<float>& noveltyFunction)
{
    noveltyFunctionSampleRate = sampleRate / tempoFFTHopLength;
    int noveltyAveragingWindowLength = noveltyFunctionSampleRate * noveltyAveragingWindowLengthInSeconds;
    
    for (int w = 0; w < numFFTWindows - 1; w++)
    {
        float localAverage = 0.0;
        int currentWindowStart = jmax<int>(w - (noveltyAveragingWindowLength * 1.0 / 2), 0);
        int currentWindowEnd = jmin<int>(w + 1 + (noveltyAveragingWindowLength * 1.0 / 2), numFFTWindows - 1);
        int currentWindowLength = currentWindowEnd - currentWindowStart;
        
        for (int m = currentWindowStart; m < currentWindowEnd; m++)
        {
            localAverage += noveltyFunction[m] / currentWindowLength;
        }
        
        // Apply half-wave rectification
        noveltyFunction[w] = jmax<float>(noveltyFunction[w] - localAverage, 0);
    }
}

std::vector<std::vector<float>> SampleAnalyser::calculateTempogram(std::vector<float> & noveltyFunction,
                                                                   int & numTempogramWindows)
{
    std::vector<std::vector<float>> spectralTempogram;
    spectralTempogram.resize(numTempi);
    int tempogramWindowLength = jmin(noveltyFunctionSampleRate * tempogramWindowLengthInSeconds,
                                     (int) noveltyFunction.size());
    int paddingLength = tempogramWindowLength / 2;
    std::vector<float> zeroPadding;
    zeroPadding.resize(paddingLength);
    memset(&*zeroPadding.begin(), 0, sizeof(float) * paddingLength);
    noveltyFunction.insert(noveltyFunction.begin(), zeroPadding.begin(), zeroPadding.end());
    noveltyFunction.insert(noveltyFunction.end(), zeroPadding.begin(), zeroPadding.end());
    numTempogramWindows = (((int) noveltyFunction.size() - tempogramWindowLength) / tempogramHopLength) + 1;
    
    // Calculate Fourier Tempogram
    mWindowFunction.fillWindowingTables(tempogramWindowLength + 1, juce::dsp::WindowingFunction<float>::hann);
    std::complex<float> i(0.0, 1.0);
    
    // Project novelty function onto each tempo sinusoid
    for (int t = 0; t < numTempi; t++)
    {
        std::vector<float> localEstimationsForCurrentTempo;
        localEstimationsForCurrentTempo.resize(numTempogramWindows);
        float beatsPerSample = ((t + lowerBPMLimitExpanded) * 1.0 / 60) / noveltyFunctionSampleRate;
        float noveltyFunctionProjection[noveltyFunction.size()];
        
        for (int s = 0; s < noveltyFunction.size(); s++)
        {
            noveltyFunctionProjection[s] = (std::exp(-2
                                                     * float (M_PI)
                                                     * i
                                                     * beatsPerSample
                                                     * float (s))
                                            * noveltyFunction[s]).real();
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
                localEstimationsForCurrentTempo[w] += windowedProjection[m];
            }
        }
        
        spectralTempogram[t] =localEstimationsForCurrentTempo;
    }
    
    return spectralTempogram;
}

std::vector<int> SampleAnalyser::calculateTempoHistogram(int numTempogramWindows, std::vector<std::vector<float>> & spectralTempogram)
{
    std::vector<int> tempoEstimationsHistogram;
    tempoEstimationsHistogram.resize(numTempi);
    
    for (int w = 0; w < numTempogramWindows; w++)
    {
        // Pick optimal tempo estimation for each position of the tempogram
        int optimalTempoIndex = 0;
        float optimalTempoBinHeight = 0.0;
        float localAverageBinHeight = 0.0;
        
        for (int t = 0; t < numTempi; t++)
        {
            float currentTempoBinHeight = spectralTempogram[t][w];
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
            tempoEstimationsHistogram[optimalTempoIndex] += 1;
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
    std::vector<float> noveltyFunction = calculateNoveltyFunction();
    
    // Subtract local average
    noveltyFunctionSubtractAverage(noveltyFunction);
    
    // Normalise novelty function
    float max = *std::max_element(noveltyFunction.begin(),
                                  noveltyFunction.begin()
                                  + numFFTWindows - 1);
    
    for (int w = 0; w < numFFTWindows - 1; w++)
    {
        noveltyFunction[w] /= max;
    }
    
    // Padding novelty function to apply a centred windowing function at position t = 0
    int numTempogramWindows;
    std::vector<std::vector<float>> spectralTempogram = calculateTempogram(noveltyFunction, numTempogramWindows);
    
    // Calculate histogram of optimal local tempo
    std::vector<int> tempoEstimationsHistogram = calculateTempoHistogram(numTempogramWindows, spectralTempogram);
    
    // Find the most prominent tempo in the histogram
    int bestTempoIndex = (int) std::distance(tempoEstimationsHistogram.begin(),
                                             std::max_element(tempoEstimationsHistogram.begin()
                                                              + ignoreTopAndBottomTempi,
                                                              tempoEstimationsHistogram.begin()
                                                              + numTempi
                                                              - ignoreTopAndBottomTempi));
    
    int bestTempoEstimation = bestTempoIndex + lowerBPMLimitExpanded;
    
    return bestTempoEstimation;
}

std::vector<std::vector<float>> SampleAnalyser::calculateLogSpectrogram(float& coefficientSum)
{
    // Apply logarithmic frequency pooling
    int numPitches = 128;
    int numPitchesPerBand = numPitches / NUM_SPECTRAL_BANDS;
    std::vector<std::vector<float>> logSpectrogram;
    logSpectrogram.resize(numPitches);
    
    // Loop over all pitch pools
    for (int p = 0; p < numPitches; p++)
    {
        float lowerFrequencyLimit = pitchToFrequency(p - 0.5);
        float upperFrequencyLimit = pitchToFrequency(p + 0.5);
        
        std::vector<float> localPitchCoefficients;
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
                float currentCoefficient = mSTFTSpectrum[w][fc];
                localPitchCoefficients[w] += currentCoefficient;
                
                // Calculate spectral distribution
                mSpectralDistribution[p / numPitchesPerBand] += currentCoefficient;
                
                // Calculate spectral spread
                float currentFrequency = (fc * sampleRate) * 1.0 / keyWindowLength;
                float meanError = currentFrequency - spectralCentroid;
                spectralSpread += (meanError * meanError) * currentCoefficient;
                coefficientSum += currentCoefficient;
            }
        }
        
        logSpectrogram[p] = localPitchCoefficients;
        
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

void SampleAnalyser::calculateChromaDistribution(std::vector<std::vector<float>> &logSpectrogram)
{
    std::memset(mChromaDistribution.data(), 0, sizeof(mChromaDistribution));
    chromaFlux = 0.0;
    
    // Loop over all pitches
    for (int p = 0; p < logSpectrogram.size(); p++)
    {
        for (int w = 0; w < numFFTWindows; w++)
        {
            float currentCoefficient = logSpectrogram[p][w];
            mChromaDistribution[p % NUM_CHROMA] += currentCoefficient;
            
            if (w >= 1)
            {
                float previousCoefficient = logSpectrogram[p][w - 1];
                chromaFlux += abs(currentCoefficient - previousCoefficient);
            }
        }
    }
    
    chromaFlux = (chromaFlux / numFFTWindows) / logSpectrogram.size();
    
    // Normalise chroma distribution
    float maxChroma = *std::max_element(mChromaDistribution.begin(),
                                        mChromaDistribution.end());
    
    for (int c = 0; c < NUM_CHROMA; c++)
    {
        mChromaDistribution[c] /= maxChroma;
    }
}

std::vector<float> SampleAnalyser::calculateKeyChromaCorrelations(float & averageCorrelation, int & numKeys)
{
    std::vector<float> keyChromaCorrelations;
    keyChromaCorrelations.resize(numKeys);
    
    averageCorrelation = 0.0;
    
    for (int k = 0; k < numKeys; k++)
    {
        std::vector<int> const & keyPattern = KEY_PATTERNS[k];
        
        for (int c = 0; c < NUM_CHROMA; c++)
        {
            if (std::find(keyPattern.begin(), keyPattern.end(), c) != keyPattern.end())
            {
                keyChromaCorrelations[k] += mChromaDistribution[c];
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
    std::memset(mSpectralDistribution.data(), 0, sizeof(mSpectralDistribution));
    
    // Calculate logarithmic spectrogram
    spectralSpread = 0.0;
    float coefficientSum = 0.0;
    std::vector<std::vector<float>> logSpectrogram = calculateLogSpectrogram(coefficientSum);
    
    // Normalise spectral distribution
    float maxSpectral = *std::max_element(mSpectralDistribution.begin(),
                                          mSpectralDistribution.end());
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
    int numKeys = (int) KEY_PATTERNS.size();
    float averageCorrelation;
    std::vector<float> keyHistogramCorrelations = calculateKeyChromaCorrelations(averageCorrelation, numKeys);
    
    // Find key with the highest histogram correlation
    averageCorrelation /= numKeys;
    int optimalKeyIndex = NO_KEY_INDEX;
    float* maxCorrelation = std::max_element(&*keyHistogramCorrelations.begin(),
                                             &*keyHistogramCorrelations.begin() + numKeys);
    
    if (*maxCorrelation > keyHistogramCorrelationThresholdFactor * averageCorrelation)
    {
        optimalKeyIndex = (int) std::distance(&*keyHistogramCorrelations.begin(), maxCorrelation);
    }
    
    return optimalKeyIndex;
}

float SampleAnalyser::pitchToFrequency(float pitchIndex, int referenceIndex, float referenceFrequency)
{
    return pow(2, (pitchIndex - referenceIndex) / NUM_CHROMA) * referenceFrequency;
}
