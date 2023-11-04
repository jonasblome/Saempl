/*
  ==============================================================================

    BlomeFileFilterRuleViewLength.h
    Created: 30 Oct 2023 7:25:18pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "BlomeFileFilterRuleViewBase.h"
#include "SampleFileFilterRuleLength.h"

/**
 The view class for \ref SampleFileFilterRuleLength.
 
 Implements:
 \ref BlomeFileFilterRuleViewBase
 \ref juce::TextEditor::Listener
 Holds a \ref juce::TextEditor to change the \ref SampleFileFilterRuleLength::mCompareValue.
 */
class BlomeFileFilterRuleViewLength
:   public BlomeFileFilterRuleViewBase,
    public TextEditor::Listener
{
public:
    /**
     The constructor of filter rule view class for the length property.
     
     @param inFilterRule the filter rule that the view should represent.
     @param inSampleLibrary the sample library of the current plugin instance.
     */
    BlomeFileFilterRuleViewLength(SampleFileFilterRuleLength& inFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewLength();
    void paint(Graphics& g) override;
    void setComponents() override;
    void resized() override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
    /**
     @returns the \ref SampleFileFilterRuleBase linked to the view.
     */
    SampleFileFilterRuleLength& getLinkedFilterRule();
    
private:
    std::unique_ptr<TextEditor> mCompareValueEditor;
};
