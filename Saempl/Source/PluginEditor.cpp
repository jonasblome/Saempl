/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SaemplAudioProcessorEditor::SaemplAudioProcessorEditor (SaemplAudioProcessor& inProcessor)
:
AudioProcessorEditor(&inProcessor),
audioProcessor(inProcessor)
{
    // Set styling of plugin UI
    mLookAndFeel = std::make_unique<BlomeLookAndFeel>(inProcessor.getSampleLibrary());
    LookAndFeel::setDefaultLookAndFeel(&*mLookAndFeel);
    
    // Set plugin window size
    setSize(style->MAIN_PANEL_WIDTH, style->MAIN_PANEL_HEIGHT);
    setResizeLimits(600, 500, 1600, 1000);
    setResizable(true, true);
    
    // Set main panel into plugin window
    mMainPanel = std::make_unique<MainPanel>(inProcessor);
    mMainPanel->setBounds(0, 0, style->MAIN_PANEL_WIDTH, style->MAIN_PANEL_HEIGHT);
    addAndMakeVisible(*mMainPanel);
}

SaemplAudioProcessorEditor::~SaemplAudioProcessorEditor()
{
    LookAndFeel::setDefaultLookAndFeel(nullptr);
}

//==============================================================================
void SaemplAudioProcessorEditor::paint(juce::Graphics& g)
{
    // The plugin background
    g.fillAll(style->COLOUR_ACCENT_DARK);
}

void SaemplAudioProcessorEditor::resized()
{
    // Setup layout of plugin components
    if (mMainPanel != nullptr)
    {
        mMainPanel->setBounds(0, 0, getWidth(), getHeight());
    }
}
