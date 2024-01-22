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
    // Higher values increase temporal resolution of the STFT spectrum
    static const int tempoFFTOrder = 10;
    static const int tempoFFTSize = 1 << tempoFFTOrder;
    static const int tempoBufferLength = 2 * tempoFFTSize;
    static const int tempoFFTHopLength = tempoBufferLength / 2;
    // Use higher values to enhance quiter elements in a sample's spectrum
    constexpr static const float compressionFactor = 10.0;
    // Higher values to decrease temporal resolution in tempogram calculation
    static const int tempogramHopLength = 1;
    // Higher values require more periodicity to trigger tempo detection
    static const int tempogramWindowLengthInSeconds = 12;
    static const int ignoreTopAndBottomTempi = 10;
    static const int lowerBPMLimit = 70 - ignoreTopAndBottomTempi;
    static const int upperBPMLimit = 180 + ignoreTopAndBottomTempi;
    static const int numTempi = upperBPMLimit - lowerBPMLimit;
    // Higher values remove more of the smallest local novelty peaks
    constexpr static const float noveltyAveragingWindowLengthInSeconds = upperBPMLimit * 1.0 / 60;
    static const int keyFFTOrder = 13;
    static const int keyFFTSize = 1 << keyFFTOrder;
    static const int keyBufferLength = 2 * keyFFTSize;
    static const int keyFFTHopLength = keyBufferLength / 2;
    static const int numChroma = 12;
    static const int loudnessBufferSize = 2048;
    int numChannels;
    int sampleRate;
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
    double analyseSampleLoudnessDecibel();
    /**
     Analyses the loudness of the given file in LUFS.
     
     @param inFile the file to analyse.
     
     @returns the loudness of the audio file in LUFS.
     */
    double analyseSampleLoudnessLUFS();
    /**
     Calculates the STFT spectrum from the loaded audio source.
     */
    void calculateSTFTSpectrum(int inBufferSize, int inFFTHopLength);
    /**
     Calculates a discrete derivative for a spectrum, applies half-wave rectification,
     accumulates the values, subtracts a local average and normalizes the final novelty function.
     
     @returns the novelty function array.
     */
    Array<float> calculateSpectralNoveltyFunction();
    /**
     Calculates a tempogram from a novelty function that covers a certain bpm range.
     
     @param noveltyFunction a reference to the novelty function array.
     @param noveltyFunctionSampleRate the rate at which the novelty function samples the original signal.
     @param numTempogramWindows the amount of windows that cover the whole novelty function.
     @param tempogramWindowLength the length of the windows for the tempogram calculation.
     
     @returns the tempogram as a 2-dimensional time-bpm array.
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
    int analyseSampleTempo();
    /**
     Analyses the key of the given file.
     
     @param inFile the file to analyse.
     
     @returns the key of the audio file.
     */
    String analyseSampleKey();
    /**
     Calculates the corresponding frequency to a pitch index according to the MIDI protocol.
     
     Reference pitch is A4 at index 69 with a reference frequency of 440 Hz.
     
     @param inPitchIndex the index of the pitch to convert.
     
     @returns the frequency of that pitch.
     */
    float pitchToFrequency(float inPitchIndex);
};
