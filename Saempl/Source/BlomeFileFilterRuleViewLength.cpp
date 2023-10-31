/*
  ==============================================================================

    BlomeFileFilterRuleViewLength.cpp
    Created: 30 Oct 2023 7:25:18pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeFileFilterRuleViewLength.h"

BlomeFileFilterRuleViewLength::BlomeFileFilterRuleViewLength(SampleFileFilterRuleLength& inFilterRule, SampleLibrary& inSampleLibrary)
:   BlomeFileFilterRuleViewBase(inSampleLibrary),
    linkedFilterRule(inFilterRule)
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
    Rectangle<int> area = getLocalBounds()
        .removeFromLeft(FILTER_RULE_TITLE_WIDTH + BUTTON_SIZE_SMALL)
        .removeFromRight(FILTER_RULE_TITLE_WIDTH);
    area.reduce(PANEL_MARGIN / 2.0, 0);
    
    g.setFont(FONT_SMALL_BOLD);
    g.setColour(COLOUR_ACCENT_LIGHT);
    g.drawFittedText(linkedFilterRule.getRuleType(),
                     area,
                     Justification::right,
                     1);
}

void BlomeFileFilterRuleViewLength::setPanelComponents()
{
    mActivateRuleButton = std::make_unique<ToggleButton>("ActivateRuleButton");
    mActivateRuleButton->setToggleState(linkedFilterRule.getIsActive(), NotificationType::dontSendNotification);
    mActivateRuleButton->onClick = [this]
    {
        linkedFilterRule.setIsActive(mActivateRuleButton->getToggleState());
    };
    addAndMakeVisible(*mActivateRuleButton);
    
    mCompareOperatorChooser = std::make_unique<ComboBox>("CompareOperatorChooser");
    mCompareOperatorChooser->addItem("is less than", 1);
    mCompareOperatorChooser->addItem("is equal to", 2);
    mCompareOperatorChooser->addItem("is greater than", 3);
    mCompareOperatorChooser->setSelectedItemIndex(linkedFilterRule.getCompareOperator());
    mCompareOperatorChooser->addListener(this);
    addAndMakeVisible(*mCompareOperatorChooser);
    
    mCompareValueEditor = std::make_unique<TextEditor>("CompareValueEditor");
    mCompareValueEditor->setFont(FONT_SMALL_BOLD);
    mCompareValueEditor->setJustification(Justification::centredLeft);
    mCompareValueEditor->setText(std::to_string(linkedFilterRule.getCompareValue()));
    mCompareValueEditor->addListener(this);
    addAndMakeVisible(*mCompareValueEditor);
    
    mDeleteRuleButton = std::make_unique<TextButton>("Delete");
    mDeleteRuleButton->onClick = [this]
    {
        
    };
    addAndMakeVisible(*mDeleteRuleButton);
}

void BlomeFileFilterRuleViewLength::resized()
{
    int deleteButtonWidth = 50;
    
    mActivateRuleButton->setBounds(FILTER_RULE_HEIGHT / 2.0
                                   - BUTTON_SIZE_SMALL / 2.0
                                   + PANEL_MARGIN / 2.0,
                                   FILTER_RULE_HEIGHT / 2.0
                                   - BUTTON_SIZE_SMALL / 2.0
                                   + PANEL_MARGIN / 4.0,
                                   BUTTON_SIZE_SMALL - PANEL_MARGIN,
                                   BUTTON_SIZE_SMALL - PANEL_MARGIN);
    
    mCompareOperatorChooser->setBounds(BUTTON_SIZE_SMALL
                                       + FILTER_RULE_TITLE_WIDTH,
                                       0,
                                       COMBO_BOX_WIDTH_MEDIUM
                                       - PANEL_MARGIN / 4.0,
                                       getHeight());
    
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
    
    mDeleteRuleButton->setBounds(getWidth() - deleteButtonWidth,
                                 0,
                                 deleteButtonWidth,
                                 getHeight());
}

void BlomeFileFilterRuleViewLength::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    switch (comboBoxThatHasChanged->getSelectedItemIndex())
    {
        case 0:
            linkedFilterRule.setCompareOperator(CompareOperators::LessThan);
            break;
        case 1:
            linkedFilterRule.setCompareOperator(CompareOperators::EqualTo);
            break;
        case 2:
            linkedFilterRule.setCompareOperator(CompareOperators::GreaterThan);
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
    linkedFilterRule.setCompareValue(textEditor.getText().getDoubleValue());
    linkedSampleLibrary.refresh();
}

SampleFileFilterRuleLength& BlomeFileFilterRuleViewLength::getLinkedFilterRule()
{
    return linkedFilterRule;
}

void BlomeFileFilterRuleViewLength::addDeleteButtonListener(Button::Listener* inListener)
{
    mDeleteRuleButton->addListener(inListener);
}

void BlomeFileFilterRuleViewLength::removeDeleteButtonListener(Button::Listener* inListener)
{
    mDeleteRuleButton->removeListener(inListener);
}
