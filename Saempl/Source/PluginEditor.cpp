/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SaemplAudioProcessorEditor::SaemplAudioProcessorEditor (SaemplAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set plugin window size
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    
    // Set main panel into plugin window
    mMainPanel = std::make_unique<MainPanel>(p);
    addAndMakeVisible(mMainPanel.get());
    
    mNoiseOverlayPanel = std::make_unique<NoiseOverlayPanel>();
    addAndMakeVisible(mNoiseOverlayPanel.get());
    
    // Set styling of plugin UI
    mLookAndFeel = std::make_unique<BlomeLookAndFeel>();
    setLookAndFeel(&*mLookAndFeel);
    juce::LookAndFeel::setDefaultLookAndFeel(&*mLookAndFeel);
}

SaemplAudioProcessorEditor::~SaemplAudioProcessorEditor()
{
}

//==============================================================================
void SaemplAudioProcessorEditor::paint (juce::Graphics& g)
{
    // The plugin background
    g.fillAll(BlomeColour_BlueishBlack);
}

void SaemplAudioProcessorEditor::resized()
{
    // Setup layout of plugin components
}