/*
 ==============================================================================
 
 BlomeFileFilterRuleViewPropertyLock.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeFileFilterRuleViewPropertyLock.h"

BlomeFileFilterRuleViewPropertyLock::BlomeFileFilterRuleViewPropertyLock(SampleFileFilterRulePropertyLock& inFilterRule, SampleLibrary& inSampleLibrary)
:
BlomeFileFilterRuleViewBase(inFilterRule, inSampleLibrary)
{
    setComponents();
}

BlomeFileFilterRuleViewPropertyLock::~BlomeFileFilterRuleViewPropertyLock()
{
    
}

void BlomeFileFilterRuleViewPropertyLock::paint(Graphics &g)
{
    BlomeFileFilterRuleViewBase::paint(g);
}

void BlomeFileFilterRuleViewPropertyLock::setComponents()
{
    // Add combo box for compare value
    mCompareValueComboBox = std::make_unique<ComboBox>("CompareValueComboBox");
    mCompareValueComboBox->addItem("On", 1);
    mCompareValueComboBox->addItem("Off", 2);
    mCompareValueComboBox->setSelectedItemIndex(getFilterRule().getCompareValue() ? 0 : 1);
    mCompareValueComboBox->addListener(this);
    addAndMakeVisible(*mCompareValueComboBox);
    
    mCompareOperatorChooser->addItem("is", 1);
    mCompareOperatorChooser->setSelectedItemIndex(0);
}

void BlomeFileFilterRuleViewPropertyLock::resized()
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

SampleFileFilterRulePropertyLock& BlomeFileFilterRuleViewPropertyLock::getFilterRule()
{
    return *dynamic_cast<SampleFileFilterRulePropertyLock*>(&filterRule);
}

void BlomeFileFilterRuleViewPropertyLock::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged->getName() == "CompareOperatorChooser")
    {
        // Set rule to chosen compare operator
        CompareOperators newOperator = static_cast<CompareOperators>(comboBoxThatHasChanged->getSelectedItemIndex() == 0 ? 1 : 3);
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
        bool newCompareValue = comboBoxThatHasChanged->getSelectedItemIndex() == 0;
        bool oldCompareValue = getFilterRule().getCompareValue();
        getFilterRule().setCompareValue(newCompareValue);
        
        if (newCompareValue != oldCompareValue && sampleLibrary.getFileFilter().canHaveEffect())
        {
            sampleLibrary.refreshLibrary();
        }
    }
}
