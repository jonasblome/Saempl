/*
 ==============================================================================
 
 BlomeFileFilterRuleViewLoudnessLUFS.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeFileFilterRuleViewLoudnessLUFS.h"

BlomeFileFilterRuleViewLoudnessLUFS::BlomeFileFilterRuleViewLoudnessLUFS(SampleFileFilterRuleLoudnessLUFS& inFilterRule, SampleLibrary& inSampleLibrary)
:
BlomeFileFilterRuleViewBase(inFilterRule, inSampleLibrary)
{
    setComponents();
}

BlomeFileFilterRuleViewLoudnessLUFS::~BlomeFileFilterRuleViewLoudnessLUFS()
{
    mCompareValueEditor->removeListener(this);
}

void BlomeFileFilterRuleViewLoudnessLUFS::paint(Graphics &g)
{
    BlomeFileFilterRuleViewBase::paint(g);
}

void BlomeFileFilterRuleViewLoudnessLUFS::setComponents()
{
    // Add text editor for compare value
    mCompareValueEditor = std::make_unique<TextEditor>("CompareValueEditor");
    mCompareValueEditor->setFont(style->FONT_SMALL_BOLD);
    mCompareValueEditor->setJustification(Justification::centredLeft);
    mCompareValueEditor->setIndents(mCompareValueEditor->getLeftIndent(), 0);
    mCompareValueEditor->setText(String::toDecimalStringWithSignificantFigures(getFilterRule().getCompareValue(), 2));
    mCompareValueEditor->addListener(this);
    addAndMakeVisible(*mCompareValueEditor);
    
    mCompareOperatorChooser->addItem("is less than", 1);
    mCompareOperatorChooser->addItem("is equal to", 2);
    mCompareOperatorChooser->addItem("is greater than", 3);
    mCompareOperatorChooser->setSelectedItemIndex(getFilterRule().getCompareOperator());
}

void BlomeFileFilterRuleViewLoudnessLUFS::resized()
{
    BlomeFileFilterRuleViewBase::resized();
    
    mCompareValueEditor->setBounds(style->BUTTON_SIZE_SMALL
                                   + style->FILTER_RULE_TITLE_WIDTH
                                   + style->COMBO_BOX_WIDTH_MEDIUM
                                   + style->PANEL_MARGIN / 4.0,
                                   0,
                                   getWidth()
                                   - style->BUTTON_SIZE_SMALL
                                   - style->FILTER_RULE_TITLE_WIDTH
                                   - style->COMBO_BOX_WIDTH_MEDIUM
                                   - style->PANEL_MARGIN * 0.75
                                   - getHeight(),
                                   getHeight());
}

void BlomeFileFilterRuleViewLoudnessLUFS::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewLoudnessLUFS::textEditorEscapeKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewLoudnessLUFS::textEditorFocusLost(TextEditor& textEditor)
{
    // Lose focus, set compare value and refresh library
    mCompareValueEditor->giveAwayKeyboardFocus();
    double newCompareValue = textEditor.getText().getDoubleValue();
    double oldCompareValue = getFilterRule().getCompareValue();
    getFilterRule().setCompareValue(newCompareValue);
    
    if (newCompareValue != oldCompareValue && sampleLibrary.getFileFilter().canHaveEffect())
    {
        sampleLibrary.refreshLibrary();
    }
}

SampleFileFilterRuleLoudnessLUFS& BlomeFileFilterRuleViewLoudnessLUFS::getFilterRule()
{
    return *dynamic_cast<SampleFileFilterRuleLoudnessLUFS*>(&filterRule);
}

void BlomeFileFilterRuleViewLoudnessLUFS::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    // Set rule to chosen compare operator
    CompareOperators newOperator = static_cast<CompareOperators>(comboBoxThatHasChanged->getSelectedItemIndex());
    CompareOperators oldOperator = filterRule.getCompareOperator();
    filterRule.setCompareOperator(newOperator);
    
    if (newOperator != oldOperator && sampleLibrary.getFileFilter().canHaveEffect())
    {
        sampleLibrary.refreshLibrary();
    }
}
