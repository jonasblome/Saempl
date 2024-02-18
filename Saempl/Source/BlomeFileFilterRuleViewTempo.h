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
 The view class for SampleFileFilterRuleTempo.
 
 Holds a TextEditor to change the compare value.
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
     @returns the rule linked to the view.
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
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
};
