/*
 ==============================================================================
 
 BlomeFileFilterRuleViewComment.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeFileFilterRuleViewComment.h"

BlomeFileFilterRuleViewComment::BlomeFileFilterRuleViewComment(SampleFileFilterRuleComment& inFilterRule, SampleLibrary& inSampleLibrary)
:
BlomeFileFilterRuleViewBase(inFilterRule, inSampleLibrary)
{
    setComponents();
}

BlomeFileFilterRuleViewComment::~BlomeFileFilterRuleViewComment()
{
    mCompareValueEditor->removeListener(this);
}

void BlomeFileFilterRuleViewComment::paint(Graphics& g)
{
    BlomeFileFilterRuleViewBase::paint(g);
}

void BlomeFileFilterRuleViewComment::setComponents()
{
    // Add text editor for compare value
    mCompareValueEditor = std::make_unique<TextEditor>("CompareValueEditor");
    mCompareValueEditor->setFont(style->FONT_SMALL_BOLD);
    mCompareValueEditor->setJustification(Justification::centredLeft);
    mCompareValueEditor->setIndents(mCompareValueEditor->getLeftIndent(), 0);
    mCompareValueEditor->setText(getFilterRule().getCompareValue());
    mCompareValueEditor->setTooltip("Enter a compare value");
    mCompareValueEditor->addListener(this);
    addAndMakeVisible(*mCompareValueEditor);
    
    mCompareOperatorChooser->addItem("is equal to", 1);
    mCompareOperatorChooser->addItem("contains", 2);
    mCompareOperatorChooser->setSelectedItemIndex(1);
}

void BlomeFileFilterRuleViewComment::resized()
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

void BlomeFileFilterRuleViewComment::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewComment::textEditorEscapeKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewComment::textEditorFocusLost(TextEditor& textEditor)
{
    // Lose focus, set compare value and refresh library
    mCompareValueEditor->giveAwayKeyboardFocus();
    String newCompareValue = textEditor.getText();
    String oldCompareValue = getFilterRule().getCompareValue();
    getFilterRule().setCompareValue(newCompareValue);
    
    if (newCompareValue != oldCompareValue && sampleLibrary.getFileFilter().canHaveEffect())
    {
        sampleLibrary.refreshLibrary();
    }
}

SampleFileFilterRuleComment& BlomeFileFilterRuleViewComment::getFilterRule()
{
    return *dynamic_cast<SampleFileFilterRuleComment*>(&filterRule);
}

void BlomeFileFilterRuleViewComment::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    // Set rule to chosen compare operator
    CompareOperators newOperator = static_cast<CompareOperators>(comboBoxThatHasChanged->getSelectedItemIndex() == 0 ? 1 : 3);
    CompareOperators oldOperator = filterRule.getCompareOperator();
    filterRule.setCompareOperator(newOperator);
    
    if (newOperator != oldOperator && sampleLibrary.getFileFilter().canHaveEffect())
    {
        sampleLibrary.refreshLibrary();
    }
}
