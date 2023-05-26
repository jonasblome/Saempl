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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    
    mMainPanel = std::make_unique<MainPanel>(&audioProcessor);
    addAndMakeVisible(mMainPanel.get());
    
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
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void SaemplAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
