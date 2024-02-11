/*
 ==============================================================================
 
 BlomeFileFilterRuleViewTitle.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeFileFilterRuleViewTitle.h"

BlomeFileFilterRuleViewTitle::BlomeFileFilterRuleViewTitle(SampleFileFilterRuleTitle& inFilterRule, SampleLibrary& inSampleLibrary)
:
BlomeFileFilterRuleViewBase(inFilterRule, inSampleLibrary)
{
    setComponents();
}

BlomeFileFilterRuleViewTitle::~BlomeFileFilterRuleViewTitle()
{
    mCompareValueEditor->removeListener(this);
}

void BlomeFileFilterRuleViewTitle::paint(Graphics &g)
{
    BlomeFileFilterRuleViewBase::paint(g);
}

void BlomeFileFilterRuleViewTitle::setComponents()
{
    // Add text editor for compare value
    mCompareValueEditor = std::make_unique<TextEditor>("CompareValueEditor");
    mCompareValueEditor->setFont(style->FONT_SMALL_BOLD);
    mCompareValueEditor->setJustification(Justification::centredLeft);
    mCompareValueEditor->setText(getFilterRule().getCompareValue());
    mCompareValueEditor->addListener(this);
    addAndMakeVisible(*mCompareValueEditor);
}

void BlomeFileFilterRuleViewTitle::resized()
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

void BlomeFileFilterRuleViewTitle::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewTitle::textEditorEscapeKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewTitle::textEditorFocusLost(TextEditor& textEditor)
{
    // Lose focus, set compare value and refresh library
    mCompareValueEditor->giveAwayKeyboardFocus();
    String newCompareValue = textEditor.getText();
    String oldCompareValue = getFilterRule().getCompareValue();
    getFilterRule().setCompareValue(newCompareValue);
    
    if (newCompareValue != oldCompareValue && getFilterRule().canHaveEffect())
    {
        sampleLibrary.refresh();
    }
}

SampleFileFilterRuleTitle& BlomeFileFilterRuleViewTitle::getFilterRule()
{
    return *dynamic_cast<SampleFileFilterRuleTitle*>(&filterRule);
}
