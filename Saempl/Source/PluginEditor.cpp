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
    // Set styling of plugin UI
    mLookAndFeel = std::make_unique<BlomeLookAndFeel>(p.getSampleLibrary());
    LookAndFeel::setDefaultLookAndFeel(&*mLookAndFeel);
    
    // Set plugin window size
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    
    // Set main panel into plugin window
    mMainPanel = std::make_unique<MainPanel>(p);
    addAndMakeVisible(mMainPanel.get());
    
    // Add noise overlay
    mNoiseOverlayPanel = std::make_unique<NoiseOverlayPanel>();
    addAndMakeVisible(mNoiseOverlayPanel.get());
}

SaemplAudioProcessorEditor::~SaemplAudioProcessorEditor()
{
}

//==============================================================================
void SaemplAudioProcessorEditor::paint (juce::Graphics& g)
{
    // The plugin background
    g.fillAll(COLOUR_ACCENT_DARK);
}

void SaemplAudioProcessorEditor::resized()
{
    // Setup layout of plugin components
}
