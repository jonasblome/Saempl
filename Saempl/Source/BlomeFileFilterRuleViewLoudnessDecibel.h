/*
  ==============================================================================

    BlomeFileFilterRuleViewLoudnessDecibel.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "BlomeFileFilterRuleViewBase.h"
#include "SampleFileFilterRuleLoudnessDecibel.h"

/**
 The view class for \ref SampleFileFilterRuleLoudnessDecibel.
 
 Implements:
 \ref BlomeFileFilterRuleViewBase
 \ref juce::TextEditor::Listener
 Holds a \ref juce::TextEditor to change the \ref SampleFileFilterRuleLoudnessDecibel::mCompareValue.
 */
class BlomeFileFilterRuleViewLoudnessDecibel
:
public BlomeFileFilterRuleViewBase,
public TextEditor::Listener
{
public:
    /**
     The constructor of filter rule view class for the decibel loudness property.
     
     @param inFilterRule the filter rule that the view should represent.
     @param inSampleLibrary the sample library of the current plugin instance.
     */
    BlomeFileFilterRuleViewLoudnessDecibel(SampleFileFilterRuleLoudnessDecibel& inFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewLoudnessDecibel();
    /**
     @returns the \ref SampleFileFilterRuleBase linked to the view.
     */
    SampleFileFilterRuleLoudnessDecibel& getFilterRule();
    
private:
    std::unique_ptr<TextEditor> mCompareValueEditor;
    
    void paint(Graphics& g) override;
    void setComponents() override;
    void resized() override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
};
