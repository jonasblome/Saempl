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
class SaemplAudioProcessor  : public juce::AudioProcessor
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
    const juce::String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
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
     Toggles between showing the library panel, the table panel and the map panel.
     
     @param inPanelType the type of the navigation panel (library, table or map).
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
     @returns the current sorting direction of the sample item table.
     */
    bool getSortingDirection();
    /**
     Sets the current sorting direction of the sample item table.
     
     @param inDirection true for fowards, false for backwards.
     */
    void setSortingDirection(bool inDirection);
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SaemplAudioProcessor)
    
    std::unique_ptr<SampleLibrary> mSampleLibrary;
    NavigationPanelType mActiveNavigationPanelType;
    String mSortingColumnTitle;
    bool mSortingDirection;
};
