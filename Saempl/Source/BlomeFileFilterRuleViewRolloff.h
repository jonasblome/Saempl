/*
 ==============================================================================
 
 BlomeFileFilterRuleViewRolloff.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "BlomeFileFilterRuleViewBase.h"
#include "SampleFileFilterRuleRolloff.h"

/**
 The view class for SampleFileFilterRuleRolloff.
 
 Holds a TextEditor to change the compare value.
 */
class BlomeFileFilterRuleViewRolloff
:
public BlomeFileFilterRuleViewBase,
public TextEditor::Listener
{
public:
    /**
     The constructor of filter rule view class for the Rolloff property.
     
     @param inFilterRule the filter rule that the view should represent.
     @param inSampleLibrary the sample library of the current plugin instance.
     */
    BlomeFileFilterRuleViewRolloff(SampleFileFilterRuleRolloff& inFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewRolloff();
    /**
     @returns the rule linked to the view.
     */
    SampleFileFilterRuleRolloff& getFilterRule();
    
private:
    std::unique_ptr<TextEditor> mCompareValueEditor;
    
    void paint(Graphics& g) override;
    void setComponents() override;
    void resized() override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlomeFileFilterRuleViewRolloff);
};
