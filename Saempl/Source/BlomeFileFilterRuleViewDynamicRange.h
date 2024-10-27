/*
 ==============================================================================
 
 BlomeFileFilterRuleViewDynamicRange.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "BlomeFileFilterRuleViewBase.h"
#include "SampleFileFilterRuleDynamicRange.h"

/**
 The view class for SampleFileFilterRuleDynamicRange.
 
 Holds a TextEditor to change the compare value.
 */
class BlomeFileFilterRuleViewDynamicRange
:
public BlomeFileFilterRuleViewBase,
public TextEditor::Listener
{
public:
    /**
     The constructor of filter rule view class for the DynamicRange property.
     
     @param inFilterRule the filter rule that the view should represent.
     @param inSampleLibrary the sample library of the current plugin instance.
     */
    BlomeFileFilterRuleViewDynamicRange(SampleFileFilterRuleDynamicRange& inFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewDynamicRange();
    /**
     @returns the rule linked to the view.
     */
    SampleFileFilterRuleDynamicRange& getFilterRule();
    
private:
    std::unique_ptr<TextEditor> mCompareValueEditor;
    
    void paint(Graphics& g) override;
    void setComponents() override;
    void resized() override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlomeFileFilterRuleViewDynamicRange);
};
