/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MainPanel.h"
#include "BlomeLookAndFeel.h"

//==============================================================================
/**
 */
class SaemplAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SaemplAudioProcessorEditor(SaemplAudioProcessor& inProcessor);
    ~SaemplAudioProcessorEditor() override;
    
    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SaemplAudioProcessorEditor)
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SaemplAudioProcessor& audioProcessor;
    std::unique_ptr<MainPanel> mMainPanel;
    std::unique_ptr<BlomeLookAndFeel> mLookAndFeel;
    SharedResourcePointer<TooltipWindow> tooltipWindow;
    BlomeStyling::StylingPtr style;
};
