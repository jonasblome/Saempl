/*
 ==============================================================================
 
 BlomeFileFilterRuleViewPropertyLock.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "BlomeFileFilterRuleViewBase.h"
#include "SampleFileFilterRulePropertyLock.h"

/**
 The view class for SampleFileFilterRulePropertyLock.
 
 Holds a TextEditor to change the compare value.
 */
class BlomeFileFilterRuleViewPropertyLock
:
public BlomeFileFilterRuleViewBase
{
public:
    /**
     The constructor of filter rule view class for the Property Lock property.
     
     @param inFilterRule the filter rule that the view should represent.
     @param inSampleLibrary the sample library of the current plugin instance.
     */
    BlomeFileFilterRuleViewPropertyLock(SampleFileFilterRulePropertyLock& inFilterRule, SampleLibrary& inSampleLibrary);
    ~BlomeFileFilterRuleViewPropertyLock();
    /**
     @returns the rule linked to the view.
     */
    SampleFileFilterRulePropertyLock& getFilterRule();
    
private:
    std::unique_ptr<ComboBox> mCompareValueComboBox;
    BlomeStyling::StylingPtr style;
    
    void paint(Graphics& g) override;
    void setComponents() override;
    void resized() override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlomeFileFilterRuleViewPropertyLock);
};
