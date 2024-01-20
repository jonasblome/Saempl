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

/**
 Analyses sample files for given categories.
 */
class SampleAnalyser
{
public:
    SampleAnalyser();
    ~SampleAnalyser();
    /**
     Analyses all properties of a given file and sets the properties in the provided SampleItem object.
     
     @param inSampleItem the sample item to set the properties for.
     */
    void analyseSample(SampleItem& inSampleItem, File const & inFile);
    
private:
    std::unique_ptr<AudioFormatReaderSource> mCurrentAudioFileSource;
    std::unique_ptr<AudioFormatManager> mFormatManager;
    AudioBuffer<float> mAnalysisBuffer;
    Ebu128LoudnessMeter mEbuLoudnessMeter;
    dsp::FFT mForwardFFT;
    dsp::WindowingFunction<float> hannWindow;
    Array<Array<float>> mSTFTSpectrum;
    Array<float> mWindowedFFTData;
    static const int fftOrder = 10; // Higher values increase temporal resolution of the STFT spectrum
    static const int fftSize = 1 << fftOrder;
    static const int bufferSize = 2 * fftSize;
    static const int fftHopLength = bufferSize / 2;
    static const int tempogramHopLength = 1; // Higher values to decrease temporal resolution in tempogram calculation
    static const int tempogramWindowLengthInSeconds = 12; // Higher values require more periodicity to trigger tempo detection
    static const int lowerBPMLimit = 60;
    static const int upperBPMLimit = 190;
    static const int numTempi = upperBPMLimit - lowerBPMLimit;
    static const int ignoreTopAndBottomTempi = 10;
    constexpr static const float compressionFactor = 1.0;
    constexpr static const float noveltyAveragingWindowLengthInSeconds = 0.3; // Higher values remove more of the smallest local novelty peaks
    int numChannels = 2;
    int sampleRate = 48000;
    int numBlocks;
    int totalNumSamples;
    int fftWindowLength;
    int numFFTWindows;
    int noveltyFunctionSampleRate;
    
    /**
     Loads the given file into a audio source.
     
     @param inFile the file to load into the audio file source.
     */
    void loadAudioFileSource(File const & inFile);
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
    /**
     Calculates the STFT spectrum from the loaded audio source.
     */
    void calculateSTFTSpectrum();
    /**
     Calculates a discrete derivative for a spectrum, applies half-wave rectification,
     accumulates the values, subtracts a local average and normalizes the final novelty function.
     */
    Array<float> calculateSpectralNoveltyFunction();
    /**
     @param noveltyFunction
     @param noveltyFunctionSampleRate
     @param numTempogramWindows
     @param tempogramWindowLength
     */
    Array<Array<float>> calculateFourierTempogram(Array<float>& noveltyFunction,
                                                  int noveltyFunctionSampleRate,
                                                  int numTempogramWindows,
                                                  int tempogramWindowLength);
    /**
     Analyses the tempo of the given file in bpm.
     
     @param inFile the file to analyse.
     
     @returns the tempo of the audio file in bpm.
     */
    int analyseSampleTempo(File const & inFile);
};
