/*
  ==============================================================================

    BlomeFileFilterRuleViewTitle.h
    Created: 30 Oct 2023 7:25:08pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "BlomeFileFilterRuleViewBase.h"
#include "SampleFileFilterRuleTitle.h"

/**
 The view class for \ref SampleFileFilterRuleTitle.
 
 Implements:
 \ref BlomeFileFilterRuleViewBase
 \ref juce::TextEditor::Listener
 Holds a \ref juce::TextEditor to change the \ref SampleFileFilterRuleTitle::mCompareValue.
 */
class BlomeFileFilterRuleViewTitle
:   public BlomeFileFilterRuleViewBase,
    public TextEditor::Listener
{
public:
    /**
     The constructor of filter rule view class for the title property.
     
     @param inFilterRule the filter rule that the view should represent.
     @param inSampleLibrary the sample library of the current plugin instance.
     */
    BlomeFileFilterRuleViewTitle(SampleFileFilterRuleTitle& inFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewTitle();
    /**
     @returns the \ref SampleFileFilterRuleBase linked to the view.
     */
    SampleFileFilterRuleTitle& getLinkedFilterRule();
    
private:
    std::unique_ptr<TextEditor> mCompareValueEditor;
    
    void paint(Graphics& g) override;
    void setComponents() override;
    void resized() override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
};
