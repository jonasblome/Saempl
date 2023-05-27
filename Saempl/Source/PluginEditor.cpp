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
    mMainPanel = std::make_unique<MainPanel>();
    addAndMakeVisible(mMainPanel.get());
    
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
    g.setColour(BlomeColour_BlueBlack);
    g.fillAll();
}

void SaemplAudioProcessorEditor::resized()
{
    // Setup layout of plugin components
}
