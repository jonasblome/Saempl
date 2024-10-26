/*
 ==============================================================================
 
 BlomeFileFilterRuleViewKey.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeFileFilterRuleViewKey.h"

BlomeFileFilterRuleViewKey::BlomeFileFilterRuleViewKey(SampleFileFilterRuleKey& inFilterRule, SampleLibrary& inSampleLibrary)
:
BlomeFileFilterRuleViewBase(inFilterRule, inSampleLibrary)
{
    setComponents();
}

BlomeFileFilterRuleViewKey::~BlomeFileFilterRuleViewKey()
{
    mCompareValueComboBox->removeListener(this);
}

void BlomeFileFilterRuleViewKey::paint(Graphics &g)
{
    BlomeFileFilterRuleViewBase::paint(g);
}

void BlomeFileFilterRuleViewKey::setComponents()
{
    // Add text editor for compare value
    mCompareValueComboBox = std::make_unique<ComboBox>("CompareValueComboBox");
    mCompareValueComboBox->setText(getFilterRule().getCompareValue());
    mCompareValueComboBox->addListener(this);
    int k = 1;
    for (auto const & key : KEY_INDEX_TO_KEY_NAME)
    {
        mCompareValueComboBox->addItem(key.second, k++);
    }
    addAndMakeVisible(*mCompareValueComboBox);
    
    mCompareOperatorChooser->addItem("is", 1);
    mCompareOperatorChooser->setSelectedItemIndex(0);
}

void BlomeFileFilterRuleViewKey::resized()
{
    BlomeFileFilterRuleViewBase::resized();
    
    mCompareValueComboBox->setBounds(style->BUTTON_SIZE_SMALL
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

SampleFileFilterRuleKey& BlomeFileFilterRuleViewKey::getFilterRule()
{
    return *dynamic_cast<SampleFileFilterRuleKey*>(&filterRule);
}

void BlomeFileFilterRuleViewKey::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged->getName() == "CompareOperatorChooser")
    {
        // Set rule to chosen compare operator
        CompareOperators newOperator = static_cast<CompareOperators>(comboBoxThatHasChanged->getSelectedItemIndex() + 1);
        CompareOperators oldOperator = filterRule.getCompareOperator();
        filterRule.setCompareOperator(newOperator);
        
        if (newOperator != oldOperator && sampleLibrary.getFileFilter().canHaveEffect())
        {
            sampleLibrary.refreshLibrary();
        }
    }
    else if (comboBoxThatHasChanged->getName() == "CompareValueComboBox")
    {
        // Set compare value and refresh library
        String newCompareValue = comboBoxThatHasChanged->getText();
        String oldCompareValue = getFilterRule().getCompareValue();
        getFilterRule().setCompareValue(newCompareValue);
        
        if (newCompareValue != oldCompareValue && sampleLibrary.getFileFilter().canHaveEffect())
        {
            sampleLibrary.refreshLibrary();
        }
    }
}
