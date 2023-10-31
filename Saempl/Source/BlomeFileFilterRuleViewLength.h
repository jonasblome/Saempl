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
    public ComboBox::Listener,
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
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
    SampleFileFilterRuleLength& getLinkedFilterRule();
    void addDeleteButtonListener(Button::Listener* inListener);
    void removeDeleteButtonListener(Button::Listener* inListener);
    
private:
    // Fields
    SampleFileFilterRuleLength& linkedFilterRule;
    std::unique_ptr<ToggleButton> mActivateRuleButton;
    std::unique_ptr<ComboBox> mCompareOperatorChooser;
    std::unique_ptr<TextEditor> mCompareValueEditor;
    std::unique_ptr<TextButton> mDeleteRuleButton;
    
    // Methods
    
};
