/*
 ==============================================================================
 
 BlomeFileFilterRuleViewZeroCrossingRate.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeFileFilterRuleViewZeroCrossingRate.h"

BlomeFileFilterRuleViewZeroCrossingRate::BlomeFileFilterRuleViewZeroCrossingRate(SampleFileFilterRuleZeroCrossingRate& inFilterRule, SampleLibrary& inSampleLibrary)
:
BlomeFileFilterRuleViewBase(inFilterRule, inSampleLibrary)
{
    setComponents();
}

BlomeFileFilterRuleViewZeroCrossingRate::~BlomeFileFilterRuleViewZeroCrossingRate()
{
    mCompareValueEditor->removeListener(this);
}

void BlomeFileFilterRuleViewZeroCrossingRate::paint(Graphics& g)
{
    BlomeFileFilterRuleViewBase::paint(g);
    
    // Paint rule unit
    Rectangle<int> area = getLocalBounds()
        .removeFromRight(style->FILTER_RULE_HEIGHT
                         + style->FILTER_RULE_UNIT_WIDTH)
        .removeFromLeft(style->FILTER_RULE_UNIT_WIDTH);
    area.reduce(style->PANEL_MARGIN / 2.0, 0);
    g.setFont(style->FONT_SMALL_BOLD);
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.drawFittedText("Hz",
                     area,
                     Justification::left,
                     1);
}

void BlomeFileFilterRuleViewZeroCrossingRate::setComponents()
{
    // Add text editor for compare value
    mCompareValueEditor = std::make_unique<TextEditor>("CompareValueEditor");
    mCompareValueEditor->setFont(style->FONT_SMALL_BOLD);
    mCompareValueEditor->setJustification(Justification::centredRight);
    mCompareValueEditor->setIndents(mCompareValueEditor->getLeftIndent(), 0);
    mCompareValueEditor->setText(std::to_string((int) getFilterRule().getCompareValue()));
    mCompareValueEditor->setTooltip("Enter a compare value");
    mCompareValueEditor->addListener(this);
    addAndMakeVisible(*mCompareValueEditor);
    
    mCompareOperatorChooser->addItem("is less than", 1);
    mCompareOperatorChooser->addItem("is equal to", 2);
    mCompareOperatorChooser->addItem("is greater than", 3);
    mCompareOperatorChooser->setSelectedItemIndex(getFilterRule().getCompareOperator());
}

void BlomeFileFilterRuleViewZeroCrossingRate::resized()
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
                                       - style->PANEL_MARGIN * 0.25
                                       - style->FILTER_RULE_UNIT_WIDTH
                                       - style->FILTER_RULE_HEIGHT,
                                   getHeight());
}

void BlomeFileFilterRuleViewZeroCrossingRate::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewZeroCrossingRate::textEditorEscapeKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewZeroCrossingRate::textEditorFocusLost(TextEditor& textEditor)
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

SampleFileFilterRuleZeroCrossingRate& BlomeFileFilterRuleViewZeroCrossingRate::getFilterRule()
{
    return *dynamic_cast<SampleFileFilterRuleZeroCrossingRate*>(&filterRule);
}

void BlomeFileFilterRuleViewZeroCrossingRate::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
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
