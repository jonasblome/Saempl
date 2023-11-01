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

class BlomeFileFilterRuleViewLength
:   public BlomeFileFilterRuleViewBase,
    public TextEditor::Listener
{
public:
    // Constructors
    BlomeFileFilterRuleViewLength(SampleFileFilterRuleLength& inFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewLength();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resized() override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
    SampleFileFilterRuleLength& getLinkedFilterRule();
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(BlomeFileFilterRuleViewLength)
    
    // Fields
    std::unique_ptr<TextEditor> mCompareValueEditor;
    
    // Methods
    
};
