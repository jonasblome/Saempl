/*
 ==============================================================================
 
 BlomeFileFilterRuleViewComment.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "BlomeFileFilterRuleViewBase.h"
#include "SampleFileFilterRuleComment.h"

/**
 The view class for SampleFileFilterRuleComment.
 
 Holds a TextEditor to change the compare value.
 */
class BlomeFileFilterRuleViewComment
:
public BlomeFileFilterRuleViewBase,
public TextEditor::Listener
{
public:
    /**
     The constructor of filter rule view class for the comment property.
     
     @param inFilterRule the filter rule that the view should represent.
     @param inSampleLibrary the sample library of the current plugin instance.
     */
    BlomeFileFilterRuleViewComment(SampleFileFilterRuleComment& inFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewComment();
    /**
     @returns the rule linked to the view.
     */
    SampleFileFilterRuleComment& getFilterRule();
    
private:
    std::unique_ptr<TextEditor> mCompareValueEditor;
    BlomeStyling::StylingPtr style;
    
    void paint(Graphics& g) override;
    void setComponents() override;
    void resized() override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlomeFileFilterRuleViewComment);
};
