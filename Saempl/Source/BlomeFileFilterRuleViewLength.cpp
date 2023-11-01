/*
  ==============================================================================

    BlomeFileFilterRuleViewLength.cpp
    Created: 30 Oct 2023 7:25:18pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeFileFilterRuleViewLength.h"

BlomeFileFilterRuleViewLength::BlomeFileFilterRuleViewLength(SampleFileFilterRuleLength& inFilterRule, SampleLibrary& inSampleLibrary)
:   BlomeFileFilterRuleViewBase(inFilterRule, inSampleLibrary)
{
    setPanelComponents();
}

BlomeFileFilterRuleViewLength::~BlomeFileFilterRuleViewLength()
{
    mCompareOperatorChooser->removeListener(this);
    mCompareValueEditor->removeListener(this);
}

void BlomeFileFilterRuleViewLength::paint(Graphics &g)
{
    BlomeFileFilterRuleViewBase::paint(g);
}

void BlomeFileFilterRuleViewLength::setPanelComponents()
{
    // Add text editor for compare value
    mCompareValueEditor = std::make_unique<TextEditor>("CompareValueEditor");
    mCompareValueEditor->setFont(FONT_SMALL_BOLD);
    mCompareValueEditor->setJustification(Justification::centredLeft);
    mCompareValueEditor->setText(std::to_string(getLinkedFilterRule().getCompareValue()));
    mCompareValueEditor->addListener(this);
    addAndMakeVisible(*mCompareValueEditor);
}

void BlomeFileFilterRuleViewLength::resized()
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
                                   - deleteButtonWidth,
                                   getHeight());
}

void BlomeFileFilterRuleViewLength::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewLength::textEditorEscapeKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewLength::textEditorFocusLost(TextEditor& textEditor)
{
    // Lose focus, set compare value and refresh library
    mCompareValueEditor->giveAwayKeyboardFocus();
    getLinkedFilterRule().setCompareValue(textEditor.getText().getIntValue());
    linkedSampleLibrary.refresh();
}

SampleFileFilterRuleLength& BlomeFileFilterRuleViewLength::getLinkedFilterRule()
{
    return *dynamic_cast<SampleFileFilterRuleLength*>(&linkedFilterRule);
}
