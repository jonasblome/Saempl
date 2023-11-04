/*
  ==============================================================================

    BlomeFileFilterRuleViewTitle.cpp
    Created: 30 Oct 2023 7:25:08pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeFileFilterRuleViewTitle.h"

BlomeFileFilterRuleViewTitle::BlomeFileFilterRuleViewTitle(SampleFileFilterRuleTitle& inFilterRule, SampleLibrary& inSampleLibrary)
:   BlomeFileFilterRuleViewBase(inFilterRule, inSampleLibrary)
{
    setComponents();
}

BlomeFileFilterRuleViewTitle::~BlomeFileFilterRuleViewTitle()
{
    
}

void BlomeFileFilterRuleViewTitle::paint(Graphics &g)
{
    BlomeFileFilterRuleViewBase::paint(g);
}

void BlomeFileFilterRuleViewTitle::setComponents()
{
    // Add text editor for compare value
    mCompareValueEditor = std::make_unique<TextEditor>("CompareValueEditor");
    mCompareValueEditor->setFont(FONT_SMALL_BOLD);
    mCompareValueEditor->setJustification(Justification::centredLeft);
    mCompareValueEditor->setText(getLinkedFilterRule().getCompareValue());
    mCompareValueEditor->addListener(this);
    addAndMakeVisible(*mCompareValueEditor);
}

void BlomeFileFilterRuleViewTitle::resized()
{
    BlomeFileFilterRuleViewBase::resized();
    
    mCompareValueEditor->setBounds(BUTTON_SIZE_SMALL
                                   + FILTER_RULE_TITLE_WIDTH
                                   + COMBO_BOX_WIDTH_MEDIUM
                                   + PANEL_MARGIN / 4.0,
                                   0,
                                   getWidth()
                                   - BUTTON_SIZE_SMALL
                                   - FILTER_RULE_TITLE_WIDTH
                                   - COMBO_BOX_WIDTH_MEDIUM
                                   - PANEL_MARGIN * 0.75
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
    getLinkedFilterRule().setCompareValue(textEditor.getText());
    linkedSampleLibrary.refresh();
}

SampleFileFilterRuleTitle& BlomeFileFilterRuleViewTitle::getLinkedFilterRule()
{
    return *dynamic_cast<SampleFileFilterRuleTitle*>(&linkedFilterRule);
}
