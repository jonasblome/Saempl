/*
  ==============================================================================

    SampleItemFilterConditionView.h
    Created: 17 Jun 2023 5:42:38pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "BlomeComponentBase.h"
#include "SampleFilterCondition.h"

class FilterConditionView
:   public BlomeComponentBase,
    public ComboBox::Listener,
    public TextEditor::Listener
{
public:
    // Constructors
    FilterConditionView(SampleFilterCondition* inFilterCondition);
    ~FilterConditionView();
    
    // Methods
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void textEditorTextChanged (TextEditor& inTextEditor) override;
    void textEditorReturnKeyPressed (TextEditor& inTextEditor) override;
    void textEditorEscapeKeyPressed (TextEditor& inTextEditor) override;
    void textEditorFocusLost (TextEditor& inTextEditor) override;
    
private:
    // Fields
    SampleFilterCondition* filterCondition;
    std::unique_ptr<ComboBox> mTagComboBox;
    std::unique_ptr<ComboBox> mCompareOperatorComboBox;
    std::unique_ptr<TextEditor> mCompareValueTextEditor;
    String currentCompareValueText;
    
    // Methods
    
};
