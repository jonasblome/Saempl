/*
  ==============================================================================

    BlomeFileFilterRuleViewLength.cpp
    Created: 30 Oct 2023 7:25:18pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeFileFilterRuleViewLength.h"

BlomeFileFilterRuleViewLength::BlomeFileFilterRuleViewLength(SampleFileFilterRuleLength& inLengthFilterRule, SampleLibrary& inSampleLibrary)
:   BlomeFileFilterRuleViewBase(inSampleLibrary),
    linkedLengthFilterRule(inLengthFilterRule)
{
    setPanelComponents();
}

BlomeFileFilterRuleViewLength::~BlomeFileFilterRuleViewLength()
{
    
}

void BlomeFileFilterRuleViewLength::paint(Graphics &g)
{
    
}

void BlomeFileFilterRuleViewLength::setPanelComponents()
{
    mCompareOperatorChooser = std::make_unique<ComboBox>("CompareOperatorChooser");
    mCompareOperatorChooser->addItem("Less than:", 1);
    mCompareOperatorChooser->addItem("Equal to:", 2);
    mCompareOperatorChooser->addItem("Greater than:", 3);
    mCompareOperatorChooser->setSelectedItemIndex(linkedLengthFilterRule.getCompareOperator());
    mCompareOperatorChooser->addListener(this);
    addAndMakeVisible(*mCompareOperatorChooser);
    
    mCompareValueEditor = std::make_unique<TextEditor>("CompareValueEditor");
    mCompareValueEditor->setFont(FONT_SMALL_BOLD);
    mCompareValueEditor->setJustification(Justification::centredLeft);
    mCompareValueEditor->setText(std::to_string(linkedLengthFilterRule.getCompareValue()));
    mCompareValueEditor->addListener(this);
    addAndMakeVisible(*mCompareValueEditor);
}

void BlomeFileFilterRuleViewLength::resized()
{
    mCompareOperatorChooser->setBounds(0,
                                       0,
                                       COMBO_BOX_WIDTH_MEDIUM - PANEL_MARGIN / 4.0,
                                       getHeight());
    
    mCompareValueEditor->setBounds(COMBO_BOX_WIDTH_MEDIUM + PANEL_MARGIN / 4.0,
                                   0,
                                   getWidth() - COMBO_BOX_WIDTH_MEDIUM - PANEL_MARGIN / 4.0,
                                   getHeight());
}

void BlomeFileFilterRuleViewLength::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    switch (comboBoxThatHasChanged->getSelectedItemIndex())
    {
        case 0:
            linkedLengthFilterRule.setCompareOperator(CompareOperators::LessThan);
            break;
        case 1:
            linkedLengthFilterRule.setCompareOperator(CompareOperators::EqualTo);
            break;
        case 2:
            linkedLengthFilterRule.setCompareOperator(CompareOperators::GreaterThan);
            break;
        default:
            break;
    }
    
    linkedSampleLibrary.refresh();
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
    mCompareValueEditor->giveAwayKeyboardFocus();
    linkedLengthFilterRule.setCompareValue(textEditor.getText().getDoubleValue());
    linkedSampleLibrary.refresh();
}
