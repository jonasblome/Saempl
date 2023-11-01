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

class BlomeFileFilterRuleViewTitle
:   public BlomeFileFilterRuleViewBase,
    public TextEditor::Listener
{
public:
    // Constructors
    BlomeFileFilterRuleViewTitle(SampleFileFilterRuleTitle& inFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewTitle();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resized() override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
    SampleFileFilterRuleTitle& getLinkedFilterRule();
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(BlomeFileFilterRuleViewTitle)
    
    // Fields
    std::unique_ptr<TextEditor> mCompareValueEditor;
    
    // Methods
    
};
