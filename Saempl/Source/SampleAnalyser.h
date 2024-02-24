/*
 ==============================================================================
 
 SampleAnalyser.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "Ebu128LoudnessMeter.h"
#include "SampleItem.h"
#include "BlomeHelpers.h"

/**
 Analyses properties of given sample files.
 */
class SampleAnalyser
{
public:
    SampleAnalyser();
    ~SampleAnalyser();
    /**
     Analyses all properties of a given file and sets the properties in the provided SampleItem object.
     
     @param inSampleItem the sample item to set the properties for.
     @param inFile the file belonging to the sample item.
     @param forceAnalysis forces analysis even for files longer than one minute.
     */
    void analyseSample(SampleItem* inSampleItem, bool forceAnalysis);
    
private:
    std::unique_ptr<AudioFormatReaderSource> mCurrentAudioFileSource;
    std::unique_ptr<AudioFormatManager> mFormatManager;
    AudioBuffer<float> mAnalysisBuffer;
    Ebu128LoudnessMeter mEbuLoudnessMeter;
    dsp::FFT mForwardFFT;
    dsp::WindowingFunction<float> mWindowFunction;
    Array<float> mWindowedFFTData;
    Array<Array<float>> mSTFTSpectrum;
    // Lower values increase temporal resolution of the STFT spectrum
    static int const tempoFFTOrder = 11;
    static int const tempoFFTSize = 1 << tempoFFTOrder;
    static int const tempoWindowLength = tempoFFTSize;
    static int const tempoFFTHopLength = tempoWindowLength / 2;
    // Higher values to decrease temporal resolution in tempogram calculation
    static int const tempogramHopLength = 1;
    // Higher values require more periodicity to trigger tempo detection
    static int const tempogramWindowLengthInSeconds = 25;
    static int const ignoreTopAndBottomTempi = 10;
    static int const lowerBPMLimit = 70 - ignoreTopAndBottomTempi;
    static int const upperBPMLimit = 180 + ignoreTopAndBottomTempi;
    static int const numTempi = upperBPMLimit - lowerBPMLimit;
    // Higher values increase frequency resolution of the STFT spectrum
    static int const keyFFTOrder = 16;
    static int const keyFFTSize = 1 << keyFFTOrder;
    static int const keyWindowLength = keyFFTSize;
    static int const keyFFTHopLength = keyWindowLength / 2;
    static int const loudnessBufferSize = 1024;
    float mSpectralDistribution[NUM_SPECTRAL_BANDS];
    float mChromaDistribution[NUM_CHROMA];
    // Change to only count local tempo optima that are above the local average
    constexpr static float const tempoAverageBinHeightThresholdFactor = 1.2;
    // Change to only count a key that has a correlation that is above the average correlation
    constexpr static float const keyHistogramCorrelationThresholdFactor = 1.03;
    // Use higher values to enhance quieter elements in a sample's spectrum
    constexpr static float const tempoCompressionFactor = 0.0;
    constexpr static float const keyCompressionFactor = 0.0;
    // Higher values remove more of the smallest local novelty peaks
    constexpr static float const noveltyAveragingWindowLengthInSeconds = 60.0f / upperBPMLimit;
    float decibel;
    float integratedLUFS;
    float lufsRangeStart;
    float lufsRangeEnd;
    float zeroCrossingRate;
    float spectralCentroid;
    float spectralSpread;
    float spectralFlux;
    float chromaFlux;
    int numChannels;
    int sampleRate;
    int numBlocks;
    int totalNumSamples;
    int fftWindowLength;
    int numFFTWindows;
    int noveltyFunctionSampleRate;
    int spectralRollOffBandIndex;
    int numZeroCrossings;
    
    /**
     Loads the given file into a audio source.
     
     @param inFile the file to load into the audio file source.
     */
    void loadAudioFileSource(File const & inFile);
    /**
     Analyses the loudness of the given file in LUFS and dB.
     */
    void analyseSampleLoudness();
    /**
     Calculates the STFT spectrum from the loaded audio source.
     
     @param inFFTOrder the order of the fft to operate on.
     @param inWindowLength the length of each analysed window.
     @param inFFTHopLength the hop length with which to shift the windows.
     @param inCompressionFactor the logarithmic compression factor of the frequency coefficients.
     */
    void calculateSTFTSpectrum(int inFFTOrder, int inBufferSize, int inFFTHopLength, float compressionFactor);
    /**
     Calculates the novelty function and the spectral centroid and flux.
     
     @returns the novelty function as an array.
     */
    Array<float> calculateNoveltyFunction();
    /**
     Subtracts the local average from a given novelty function.
     
     @param noveltyFunction the novelty function to subtract the average from.
     */
    void noveltyFunctionSubtractAverage(Array<float>& noveltyFunction);
    /**
     Calculates a tempogram from a given novelty function.
     
     @param noveltyFunction the function to calculate the tempogram from.
     @param numTempogramWindows the amount of windows in the tempogram.
     
     @returns the tempogram as a 2d array.
     */
    Array<Array<float>> calculateTempogram(Array<float>& noveltyFunction, int& numTempogramWindows);
    /**
     Calculates a tempo histogram with the optimal pitch at each position from a given tempogram.
     
     @param numTempogramWindows the amount of windows in the tempogram.
     @param spectralTempogram the tempogram to calculate the histogram from
     
     @returns histogram of optimal tempi in the tempogram.
     */
    Array<int> calculateTempoHistogram(int numTempogramWindows, Array<Array<float>>& spectralTempogram);
    /**
     Analyses the tempo of the given file in bpm.
     
     @param inFile the file to analyse.
     
     @returns the tempo of the audio file in bpm.
     */
    int analyseSampleTempo();
    /**
     Calculates a spectrogram with a logarithmic frequency axis, representing the MIDI pitches.
     
     @param coefficientSum the sum of all coefficient amplitudes in the spectrogram.
     
     @returns the log-spectrogram.
     */
    Array<Array<float>> calculateLogSpectrogram(float& coefficientSum);
    /**
     Calculates the distribution of chroma in the given spectrogram.
     
     @param logSpectrogram the spectrogram to calculate the chroma distribution from.
     */
    void calculateChromaDistribution(Array<Array<float>>& logSpectrogram);
    /**
     Calculates the correlation between the chroma distribution and the index list for each key.
     
     @param averageCorrelation the output variable for the average key chroma correlation.
     @param numKeys the amount of keys to correlate with.
     */
    Array<float> calculateKeyChromaCorrelations(float& averageCorrelation, int& numKeys);
    /**
     Analyses the key of the given file.
     
     @param inFile the file to analyse.
     
     @returns the key of the audio file.
     */
    int analyseSampleKey();
    /**
     Calculates the corresponding frequency to a pitch index according to the MIDI protocol.
     
     Reference pitch is A4 at index 69 with a reference frequency of 440 Hz.
     
     @param inPitchIndex the index of the pitch to convert.
     @param referenceIndex the index of the reference pitch.
     @param referenceFrequency the frequency of the reference pitch in Hertz.
     
     @returns the frequency of that pitch.
     */
    float pitchToFrequency(float inPitchIndex, int referenceIndex = 69, float referenceFrequency = 440.0);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleAnalyser);
};
