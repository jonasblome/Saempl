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
    BlomeFileFilterRuleViewTitle(SampleFileFilterRuleTitle& inFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewTitle();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resized() override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
    SampleFileFilterRuleTitle& getLinkedFilterRule();
    void addDeleteButtonListener(Button::Listener* inListener);
    void removeDeleteButtonListener(Button::Listener* inListener);
    
private:
    // Fields
    SampleFileFilterRuleTitle& linkedFilterRule;
    std::unique_ptr<ToggleButton> mActivateRuleButton;
    std::unique_ptr<ComboBox> mCompareOperatorChooser;
    std::unique_ptr<TextEditor> mCompareValueEditor;
    std::unique_ptr<TextButton> mDeleteRuleButton;
    
    // Methods
    
};
