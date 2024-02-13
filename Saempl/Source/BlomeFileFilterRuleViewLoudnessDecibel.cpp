/*
  ==============================================================================

    BlomeFileFilterRuleViewLoudnessDecibel.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeFileFilterRuleViewLoudnessDecibel.h"

BlomeFileFilterRuleViewLoudnessDecibel::BlomeFileFilterRuleViewLoudnessDecibel(SampleFileFilterRuleLoudnessDecibel& inFilterRule, SampleLibrary& inSampleLibrary)
:
BlomeFileFilterRuleViewBase(inFilterRule, inSampleLibrary)
{
    setComponents();
}

BlomeFileFilterRuleViewLoudnessDecibel::~BlomeFileFilterRuleViewLoudnessDecibel()
{
    mCompareValueEditor->removeListener(this);
}

void BlomeFileFilterRuleViewLoudnessDecibel::paint(Graphics &g)
{
    BlomeFileFilterRuleViewBase::paint(g);
}

void BlomeFileFilterRuleViewLoudnessDecibel::setComponents()
{
    // Add text editor for compare value
    mCompareValueEditor = std::make_unique<TextEditor>("CompareValueEditor");
    mCompareValueEditor->setFont(style->FONT_SMALL_BOLD);
    mCompareValueEditor->setJustification(Justification::centredLeft);
    mCompareValueEditor->setText(std::to_string(getFilterRule().getCompareValue()));
    mCompareValueEditor->addListener(this);
    addAndMakeVisible(*mCompareValueEditor);
}

void BlomeFileFilterRuleViewLoudnessDecibel::resized()
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

void BlomeFileFilterRuleViewLoudnessDecibel::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewLoudnessDecibel::textEditorEscapeKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void BlomeFileFilterRuleViewLoudnessDecibel::textEditorFocusLost(TextEditor& textEditor)
{
    // Lose focus, set compare value and refresh library
    mCompareValueEditor->giveAwayKeyboardFocus();
    float newCompareValue = textEditor.getText().getFloatValue();
    float oldCompareValue = getFilterRule().getCompareValue();
    getFilterRule().setCompareValue(newCompareValue);
    
    if (newCompareValue != oldCompareValue && sampleLibrary.getFileFilter().canHaveEffect())
    {
        sampleLibrary.refresh();
    }
}

SampleFileFilterRuleLoudnessDecibel& BlomeFileFilterRuleViewLoudnessDecibel::getFilterRule()
{
    return *dynamic_cast<SampleFileFilterRuleLoudnessDecibel*>(&filterRule);
}
