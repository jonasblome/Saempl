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
    public ComboBox::Listener,
    public TextEditor::Listener
{
public:
    // Constructors
    BlomeFileFilterRuleViewTitle(SampleFileFilterRuleTitle& inTitleFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewTitle();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resized() override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
    
private:
    // Fields
    SampleFileFilterRuleTitle& linkedTitleFilterRule;
    std::unique_ptr<ComboBox> mCompareOperatorChooser;
    std::unique_ptr<TextEditor> mCompareValueEditor;
    
    // Methods
    
};
