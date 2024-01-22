/*
  ==============================================================================

    BlomeFileFilterRuleViewKey.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "BlomeFileFilterRuleViewBase.h"
#include "SampleFileFilterRuleKey.h"

/**
 The view class for SampleFileFilterRuleKey.
 
 Holds a juce::TextEditor to change the SampleFileFilterRuleKey::mCompareValue.
 */
class BlomeFileFilterRuleViewKey
:
public BlomeFileFilterRuleViewBase,
public TextEditor::Listener
{
public:
    /**
     The constructor of filter rule view class for the key property.
     
     @param inFilterRule the filter rule that the view should represent.
     @param inSampleLibrary the sample library of the current plugin instance.
     */
    BlomeFileFilterRuleViewKey(SampleFileFilterRuleKey& inFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewKey();
    /**
     @returns the SampleFileFilterRuleBase linked to the view.
     */
    SampleFileFilterRuleKey& getFilterRule();
    
private:
    std::unique_ptr<TextEditor> mCompareValueEditor;
    
    void paint(Graphics& g) override;
    void setComponents() override;
    void resized() override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
};
