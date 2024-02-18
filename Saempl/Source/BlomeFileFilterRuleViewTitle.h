/*
 ==============================================================================
 
 BlomeFileFilterRuleViewTitle.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "BlomeFileFilterRuleViewBase.h"
#include "SampleFileFilterRuleTitle.h"

/**
 The view class for SampleFileFilterRuleTitle.
 
 Holds a TextEditor to change the compare value.
 */
class BlomeFileFilterRuleViewTitle
:
public BlomeFileFilterRuleViewBase,
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
     @returns the rule linked to the view.
     */
    SampleFileFilterRuleTitle& getFilterRule();
    
private:
    std::unique_ptr<TextEditor> mCompareValueEditor;
    BlomeStyling::Ptr style;
    
    void paint(Graphics& g) override;
    void setComponents() override;
    void resized() override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
};
