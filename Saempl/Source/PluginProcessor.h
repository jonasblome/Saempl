/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "SampleLibrary.h"

//==============================================================================
/**
 */
class SaemplAudioProcessor
:
public juce::AudioProcessor
#if JucePlugin_Enable_ARA
, public juce::AudioProcessorARAExtension
#endif
{
public:
    //==============================================================================
    SaemplAudioProcessor();
    ~SaemplAudioProcessor() override;
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    juce::String const getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    juce::String const getProgramName (int index) override;
    void changeProgramName (int index, juce::String const & newName) override;
    
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (void const * data, int sizeInBytes) override;
    
    //==============================================================================
    /**
     @returns the sample library of the current plugin instance.
     */
    SampleLibrary& getSampleLibrary();
    /**
     @returns the type of the currently shown navigation panel.
     */
    NavigationPanelType& getActiveNavigationPanel();
    /**
     Toggles between showing the library panel, the table panel and the grid panel.
     
     @param inPanelType the type of the navigation panel (library, table or grid).
     */
    void setActiveNavigationPanel(NavigationPanelType& inPanelType);
    /**
     @returns the title of the current sorting column of the sample item table.
     */
    String getSortingColumnTitle();
    /**
     Sets the title of the current sorting column of the sample item table.
     */
    void setSortingColumnTitle(String inColumnTitle);
    /**
     @returns the path of the last opened library directory.
     */
    String getLastOpenedLibraryPath();
    /**
     Sets the path of the last opened library directory.
     */
    void setLastOpenedLibraryPath(String inLastOpenedLibraryPath);
    /**
     @returns the current sorting direction of the sample item table.
     */
    bool getSortingDirection();
    /**
     Sets the current sorting direction of the sample item table.
     
     @param inDirection true for fowards, false for backwards.
     */
    void setSortingDirection(bool inDirection);
    /**
     @returns the current visibility of the sample preview panel.
     */
    bool getSampleItemPanelIsVisible();
    /**
     Sets the current visibility of the sample preview panel.
     
     @param inPanelIsVisible whether the panel is visible or not.
     */
    void setSampleItemIsVisible(bool inPanelIsVisible);
    /**
     Sets the follow playhead toggle of the sample preview panel.
     
     @param inPanelIsVisible whether the panel is visible or not.
     */
    void setFollowAudioPlayhead(bool inFollowAudioPlayhead);
    /**
     @returns the current toggle state of the sample preview panel's follow playhead.
     */
    bool getFollowAudioPlayhead();
    /**
     Sets the filter toggle state.
     
     @param inPanelIsVisible whether the panel is visible or not.
     */
    void setFilterIsActive(bool inFilterIsActivated);
    /**
     @returns the current toggle state of the sample filter.
     */
    bool getFilterIsActivated();
    /**
     @returns the current zoom factor of the sample grid panel.
     */
    float getSampleGridZoomFactor();
    /**
     Sets the current zoom factor of the sample grid panel.
     
     @param inZoomFactor the zoom factor of the grid panel's zoom slider.
     */
    void setSampleGridZoomFactor(float inZoomFactor);
    /**
     @returns the current output gain.
     */
    float getOutputGain();
    /**
     Sets the current output gain.
     
     @param inGain the output gain.
     */
    void setOutputGain(float inGain);
    /**
     @returns the feature weights for the grid clusterer.
     */
    std::vector<float> getFeatureWeights();
    /**
     Sets the feature weights for the grid clusterer.
     
     @param inFeatureWeights the feature weights.
     */
    void setFeatureWeights(std::vector<float> inFeatureWeights);
    /**
     @returns whether the feature weights changed since the last clustering.
     */
    bool getFeatureWeightsChanged();
    /**
     Sets whether the feature weights changed since the last clustering.
     
     @param inFeatureWeightsChanged whether the feature weights changed since the last clustering.
     */
    void setFeatureWeightsChanged(bool inFeatureWeightsChanged);
    /**
     @returns whether the audio playback volume is normalised.
     */
    bool getVolumeIsNormalised();
    /**
     Sets whether the audio playback volume is normalised.
     
     @param inVolumeIsNormalised whether the audio playback volume is normalised.
     */
    void setVolumeIsNormalised(bool inVolumeIsNormalised);
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SaemplAudioProcessor)
    
    std::unique_ptr<SampleLibrary> mSampleLibrary;
    NavigationPanelType mActiveNavigationPanelType;
    String mDefaultLibraryDirectoryPath =
    (File::getSpecialLocation(File::userMusicDirectory)).getFullPathName()
    + DIRECTORY_SEPARATOR
    + "Plugins"
    + DIRECTORY_SEPARATOR
    + "Saempl"
    + DIRECTORY_SEPARATOR
    + "DefaultSampleLibrary";
    String mSortingColumnTitle;
    String mLastOpenedLibraryPath;
    bool mSortingDirection;
    bool mSampleItemPanelIsVisible;
    bool mFollowAudioPlayhead;
    bool mFilterIsActivated;
    bool mFeatureWeightsChanged;
    bool mVolumeIsNormalised;
    float mSampleGridZoomFactor;
    float mOutputGain;
    std::vector<float> mFeatureWeights;
};
