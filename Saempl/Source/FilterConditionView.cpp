/*
  ==============================================================================

    SampleItemFilterConditionView.cpp
    Created: 17 Jun 2023 5:42:38pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "FilterConditionView.h"

FilterConditionView::FilterConditionView(SampleFilterCondition* inFilterCondition)
:   filterCondition(inFilterCondition)
{
    // Set tag combobox
    int x = 0;
    mTagComboBox = std::make_unique<ComboBox>("TagComboBox");
    mTagComboBox->setBounds(x, 0, getWidth(), getHeight());
    
    for (int t = 0; t < AnalysisTypes.size(); t++)
    {
        mTagComboBox->addItem(AnalysisTypes[t], t + 1);
    }
    
    // Search for index of current name tag
    int key = 0;
    for (auto &i : AnalysisTypes) {
        if (i.second == filterCondition->getNameTagToCheck()) {
            key = i.first;
        }
    }
    
    mTagComboBox->setSelectedItemIndex(key + 1);
    mTagComboBox->addListener(this);
    addAndMakeVisible(*mTagComboBox);
    
    // Set compare operator combobox
    x += 80;
    mCompareOperatorComboBox = std::make_unique<ComboBox>("CompareOperatorComboBox");
    mCompareOperatorComboBox->setBounds(x, 0, getWidth(), getHeight());
    
    for (int compareOperatorInt = CompareOperator_Equals; compareOperatorInt != CompareOperator_Final; compareOperatorInt++)
    {
        mCompareOperatorComboBox->addItem(compareOperatorToString(static_cast<CompareOperator>(compareOperatorInt)), compareOperatorInt + 1);
    }
    
    mCompareOperatorComboBox->setSelectedItemIndex(key + 1);
    mCompareOperatorComboBox->addListener(this);
    addAndMakeVisible(*mCompareOperatorComboBox);
    
    // Set compare value text editor
    x += 80;
    mCompareValueTextEditor = std::make_unique<TextEditor>();
    mCompareValueTextEditor->setBounds(x, 0, getWidth(), getHeight());
    mCompareValueTextEditor->setText(String(filterCondition->getCompareValue()));
    currentCompareValueText = mCompareValueTextEditor->getText();
    mCompareValueTextEditor->addListener(this);
    addAndMakeVisible(*mCompareValueTextEditor);
}

FilterConditionView::~FilterConditionView()
{
    mTagComboBox->removeListener(this);
    mCompareOperatorComboBox->removeListener(this);
    mCompareValueTextEditor->removeListener(this);
}

void FilterConditionView::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &*mTagComboBox)
    {
        filterCondition->setNameTagToCheck(AnalysisTypes[comboBoxThatHasChanged->getSelectedItemIndex() - 1]);
    }
}

void FilterConditionView::textEditorTextChanged (TextEditor& inTextEditor)
{
    
}

void FilterConditionView::textEditorReturnKeyPressed (TextEditor& inTextEditor)
{
    juce::TextEditor::Listener::textEditorFocusLost(inTextEditor);
}

void FilterConditionView::textEditorEscapeKeyPressed (TextEditor& inTextEditor)
{
    inTextEditor.setText(currentCompareValueText);
}

void FilterConditionView::textEditorFocusLost (TextEditor& inTextEditor)
{
    currentCompareValueText = inTextEditor.getText();
    filterCondition->setCompareValue(currentCompareValueText.getDoubleValue());
}
