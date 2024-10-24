/*
 ==============================================================================
 
 BlomeFileFilterRuleViewBase.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "BlomeStyling.h"
#include "SampleLibrary.h"
#include "BlomeImageButton.h"

/**
 The base class for views to the SampleFileFilterRuleBase classes.
 
 The component holds a button for toggling the filter rule's active state,
 a combo box for choosing the SampleFileFilterRuleBase::mCompareOperator
 and a delete button to remove the filter rule.
 */
class BlomeFileFilterRuleViewBase
:
public Component,
public ComboBox::Listener
{
public:
    /**
     The constructor of filter rule view base class.
     
     @param inFilterRule the filter rule that the view should represent.
     @param inSampleLibrary the sample library of the current plugin instance.
     */
    BlomeFileFilterRuleViewBase(SampleFileFilterRuleBase& inFilterRule, SampleLibrary& inSampleLibrary);
    virtual ~BlomeFileFilterRuleViewBase();
    /**
     @returns the SampleFileFilterRuleBase linked to the view.
     */
    SampleFileFilterRuleBase& getFilterRule();
    /**
     Adds a listener to the mDeleteRuleButton.
     @param inListener the listener object to add.
     */
    void addDeleteButtonListener(Button::Listener* inListener);
    /**
     Removes a listener from the mDeleteRuleButton.
     @param inListener the listener object to remove.
     */
    void removeDeleteButtonListener(Button::Listener* inListener);
    
protected:
    SampleLibrary& sampleLibrary;
    SampleFileFilterRuleBase& filterRule;
    std::unique_ptr<ComboBox> mCompareOperatorChooser;
    std::unique_ptr<ToggleButton> mActivateRuleButton;
    std::unique_ptr<BlomeImageButton> mDeleteRuleButton;
    BlomeStyling::StylingPtr style;
    
    /**
     Initialises the panel's components and adds them to the parent component.
     */
    virtual void setComponents();
    void paint(Graphics& g) override;
    void resized() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlomeFileFilterRuleViewBase);
};
