/*
 ==============================================================================
 
 BlomeFileFilterRuleViewBase.cpp
 Created: 29 Oct 2023 12:14:30pm
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeFileFilterRuleViewBase.h"

BlomeFileFilterRuleViewBase::BlomeFileFilterRuleViewBase(SampleFileFilterRuleBase& inFilterRule,
                                                         SampleLibrary& inSampleLibrary)
:   linkedSampleLibrary(inSampleLibrary),
linkedFilterRule(inFilterRule)
{
    setComponents();
}

BlomeFileFilterRuleViewBase::~BlomeFileFilterRuleViewBase()
{
    
}

void BlomeFileFilterRuleViewBase::paint(Graphics &g)
{
    // Paint rule title
    Rectangle<int> area = getLocalBounds()
        .removeFromLeft(FILTER_RULE_TITLE_WIDTH + BUTTON_SIZE_SMALL)
        .removeFromRight(FILTER_RULE_TITLE_WIDTH);
    area.reduce(PANEL_MARGIN / 2.0, 0);
    g.setFont(FONT_SMALL_BOLD);
    g.setColour(COLOUR_ACCENT_LIGHT);
    g.drawFittedText(getLinkedFilterRule().getRulePropertyName(),
                     area,
                     Justification::right,
                     1);
}

void BlomeFileFilterRuleViewBase::setComponents()
{
    // Add button to activate filter rule
    mActivateRuleButton = std::make_unique<ToggleButton>("ActivateRuleButton");
    mActivateRuleButton->setToggleState(linkedFilterRule.getIsActive(), NotificationType::dontSendNotification);
    mActivateRuleButton->setTooltip("Activate/deactivate this filter rule");
    mActivateRuleButton->onClick = [this]
    {
        linkedFilterRule.setIsActive(mActivateRuleButton->getToggleState());
        linkedSampleLibrary.refresh();
    };
    addAndMakeVisible(*mActivateRuleButton);
    
    // Add combo box for compare operator
    mCompareOperatorChooser = std::make_unique<ComboBox>("CompareOperatorChooser");
    mCompareOperatorChooser->addItem("is less than", 1);
    mCompareOperatorChooser->addItem("is equal to", 2);
    mCompareOperatorChooser->addItem("is greater than", 3);
    mCompareOperatorChooser->addItem("contains", 4);
    mCompareOperatorChooser->setSelectedItemIndex(getLinkedFilterRule().getCompareOperator());
    mCompareOperatorChooser->addListener(this);
    addAndMakeVisible(*mCompareOperatorChooser);
    
    // Add button for deleting filter rule
    mDeleteRuleButton = std::make_unique<BlomeImageButton>("Delete", true);
    mDeleteRuleButton->setImages(false,
                                 true,
                                 true,
                                 ImageCache::getFromMemory(BinaryData::delete_FILL0_wght400_GRAD0_opsz24_png,
                                                           BinaryData::delete_FILL0_wght400_GRAD0_opsz24_pngSize),
                                 BUTTON_IS_DEFAULT_ALPHA,
                                 COLOUR_ACCENT_LIGHT,
                                 Image(),
                                 BUTTON_IS_OVER_ALPHA,
                                 Colour(),
                                 Image(),
                                 BUTTON_IS_DOWN_ALPHA,
                                 Colour());
    mDeleteRuleButton->setTooltip("Delete this filter rule");
    mDeleteRuleButton->onClick = [this]
    {
        
    };
    addAndMakeVisible(*mDeleteRuleButton);
}

void BlomeFileFilterRuleViewBase::resized()
{
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
    
    mDeleteRuleButton->setBounds(getWidth() - getHeight(),
                                 0,
                                 getHeight(),
                                 getHeight());
}

void BlomeFileFilterRuleViewBase::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    // Set rule to chosen compare operator
    switch (comboBoxThatHasChanged->getSelectedItemIndex())
    {
        case 0:
            linkedFilterRule.setCompareOperator(CompareOperators::LESS_THAN);
            break;
        case 1:
            linkedFilterRule.setCompareOperator(CompareOperators::EQUAL_TO);
            break;
        case 2:
            linkedFilterRule.setCompareOperator(CompareOperators::GREATER_THAN);
            break;
        case 3:
            linkedFilterRule.setCompareOperator(CompareOperators::CONTAINS);
            break;
        default:
            jassertfalse;
            break;
    }
    
    linkedSampleLibrary.refresh();
}

SampleFileFilterRuleBase& BlomeFileFilterRuleViewBase::getLinkedFilterRule()
{
    return linkedFilterRule;
}

void BlomeFileFilterRuleViewBase::addDeleteButtonListener(Button::Listener* inListener)
{
    mDeleteRuleButton->addListener(inListener);
}

void BlomeFileFilterRuleViewBase::removeDeleteButtonListener(Button::Listener* inListener)
{
    mDeleteRuleButton->removeListener(inListener);
}