/*
 ==============================================================================
 
 BlomeFileFilterRuleViewLoudnessLUFS.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "BlomeFileFilterRuleViewBase.h"
#include "SampleFileFilterRuleLoudnessLUFS.h"

/**
 The view class for SampleFileFilterRuleLoudnessLUFS.
 
 Holds a TextEditor to change the compare value.
 */
class BlomeFileFilterRuleViewLoudnessLUFS
:
public BlomeFileFilterRuleViewBase,
public TextEditor::Listener
{
public:
    /**
     The constructor of filter rule view class for the LUFS loudness property.
     
     @param inFilterRule the filter rule that the view should represent.
     @param inSampleLibrary the sample library of the current plugin instance.
     */
    BlomeFileFilterRuleViewLoudnessLUFS(SampleFileFilterRuleLoudnessLUFS& inFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewLoudnessLUFS();
    /**
     @returns the rule linked to the view.
     */
    SampleFileFilterRuleLoudnessLUFS& getFilterRule();
    
private:
    std::unique_ptr<TextEditor> mCompareValueEditor;
    
    void paint(Graphics& g) override;
    void setComponents() override;
    void resized() override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlomeFileFilterRuleViewLoudnessLUFS);
};
