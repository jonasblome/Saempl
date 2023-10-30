/*
  ==============================================================================

    BlomeFileFilterRuleViewTitle.cpp
    Created: 30 Oct 2023 7:25:08pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeFileFilterRuleViewTitle.h"

BlomeFileFilterRuleViewTitle::BlomeFileFilterRuleViewTitle(SampleFileFilterRuleTitle& inTitleFilterRule, SampleLibrary& inSampleLibrary)
:   BlomeFileFilterRuleViewBase(inSampleLibrary),
    linkedTitleFilterRule(inTitleFilterRule)
{
    setPanelComponents();
}

BlomeFileFilterRuleViewTitle::~BlomeFileFilterRuleViewTitle()
{
    
}

void BlomeFileFilterRuleViewTitle::paint(Graphics &g)
{
    
}

void BlomeFileFilterRuleViewTitle::setPanelComponents()
{
    mCompareOperatorChooser = std::make_unique<ComboBox>("CompareOperatorChooser");
    mCompareOperatorChooser->addItem("Less than:", 1);
    mCompareOperatorChooser->addItem("Equal to:", 2);
    mCompareOperatorChooser->addItem("Greater than:", 3);
    mCompareOperatorChooser->setSelectedItemIndex(linkedTitleFilterRule.getCompareOperator());
    mCompareOperatorChooser->addListener(this);
    addAndMakeVisible(*mCompareOperatorChooser);
    
    mCompareValueEditor = std::make_unique<TextEditor>("CompareValueEditor");
    mCompareValueEditor->setFont(FONT_SMALL_BOLD);
    mCompareValueEditor->setJustification(Justification::centredLeft);
    mCompareValueEditor->setText(linkedTitleFilterRule.getCompareValue());
    mCompareValueEditor->addListener(this);
    addAndMakeVisible(*mCompareValueEditor);
}

void BlomeFileFilterRuleViewTitle::resized()
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

void BlomeFileFilterRuleViewTitle::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    switch (comboBoxThatHasChanged->getSelectedItemIndex())
    {
        case 0:
            linkedTitleFilterRule.setCompareOperator(CompareOperators::LessThan);
            break;
        case 1:
            linkedTitleFilterRule.setCompareOperator(CompareOperators::EqualTo);
            break;
        case 2:
            linkedTitleFilterRule.setCompareOperator(CompareOperators::GreaterThan);
            break;
        default:
            break;
    }
    
    linkedSampleLibrary.refresh();
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
    mCompareValueEditor->giveAwayKeyboardFocus();
    linkedTitleFilterRule.setCompareValue(textEditor.getText());
    linkedSampleLibrary.refresh();
}
