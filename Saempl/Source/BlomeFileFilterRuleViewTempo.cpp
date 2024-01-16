/*
  ==============================================================================

    BlomeFileFilterRuleViewTempo.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeFileFilterRuleViewTempo.h"

BlomeFileFilterRuleViewTempo::BlomeFileFilterRuleViewTempo(SampleFileFilterRuleTempo& inFilterRule, SampleLibrary& inSampleLibrary)
:
BlomeFileFilterRuleViewBase(inFilterRule, inSampleLibrary)
{
    setComponents();
}

BlomeFileFilterRuleViewTempo::~BlomeFileFilterRuleViewTempo()
{
    mCompareValueEditor->removeListener(this);
}

void BlomeFileFilterRuleViewTempo::paint(Graphics &g)
{
    BlomeFileFilterRuleViewBase::paint(g);
}

void BlomeFileFilterRuleViewTempo::setComponents()
{
    // Add text editor for compare value
    mCompareValueEditor = std::make_unique<TextEditor>("CompareValueEditor");
    mCompareValueEditor->setFont(style->FONT_SMALL_BOLD);
    mCompareValueEditor->setJustification(Justification::centredLeft);
    mCompareValueEditor->setText(std::to_string(getFilterRule().getCompareValue()));
    mCompareValueEditor->addListener(this);
    addAndMakeVisible(*mCompareValueEditor);
}

void BlomeFileFilterRuleViewTempo::resized()
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

void BlomeFileFilterRuleViewTempo::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewTempo::textEditorEscapeKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewTempo::textEditorFocusLost(TextEditor& textEditor)
{
    // Lose focus, set compare value and refresh library
    mCompareValueEditor->giveAwayKeyboardFocus();
    getFilterRule().setCompareValue(textEditor.getText().getIntValue());
    sampleLibrary.refresh();
}

SampleFileFilterRuleTempo& BlomeFileFilterRuleViewTempo::getFilterRule()
{
    return *dynamic_cast<SampleFileFilterRuleTempo*>(&filterRule);
}
