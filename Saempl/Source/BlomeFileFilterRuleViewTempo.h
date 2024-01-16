/*
  ==============================================================================

    BlomeFileFilterRuleViewTempo.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "BlomeFileFilterRuleViewBase.h"
#include "SampleFileFilterRuleTempo.h"

/**
 The view class for \ref SampleFileFilterRuleTempo.
 
 Implements:
 \ref BlomeFileFilterRuleViewBase
 \ref juce::TextEditor::Listener
 Holds a \ref juce::TextEditor to change the \ref SampleFileFilterRuleTempo::mCompareValue.
 */
class BlomeFileFilterRuleViewTempo
:
public BlomeFileFilterRuleViewBase,
public TextEditor::Listener
{
public:
    /**
     The constructor of filter rule view class for the tempo property.
     
     @param inFilterRule the filter rule that the view should represent.
     @param inSampleLibrary the sample library of the current plugin instance.
     */
    BlomeFileFilterRuleViewTempo(SampleFileFilterRuleTempo& inFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewTempo();
    /**
     @returns the \ref SampleFileFilterRuleBase linked to the view.
     */
    SampleFileFilterRuleTempo& getFilterRule();
    
private:
    std::unique_ptr<TextEditor> mCompareValueEditor;
    
    void paint(Graphics& g) override;
    void setComponents() override;
    void resized() override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
};
