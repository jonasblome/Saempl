/*
  ==============================================================================

    BlomeFileFilterRuleViewBase.h
    Created: 29 Oct 2023 12:14:30pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "BlomeStyleDefinitions.h"
#include "SampleLibrary.h"

/**
 The base class for views to the \ref SampleFileFilterRuleBase classes.
 
 Implements:
 \ref juce::Component
 \ref juce::ComboBox::Listener
 The component holds a button for toggling the filter rule's active state,
 a combo box for choosing the \ref SampleFileFilterRuleBase::mCompareOperator
 and a delete button to remove the filter rule.
 */
class BlomeFileFilterRuleViewBase
:   public Component,
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
    void paint(Graphics& g) override;
    /**
     Initializes the panel's components and adds them to the parent component.
     */
    virtual void setComponents();
    void resized() override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    /**
     @returns the \ref SampleFileFilterRuleBase linked to the view.
     */
    SampleFileFilterRuleBase& getLinkedFilterRule();
    /**
     Adds a listener to the \ref mDeleteRuleButton.
     @param inListener the listener object to add.
     */
    void addDeleteButtonListener(Button::Listener* inListener);
    /**
     Removes a listener from the \ref mDeleteRuleButton.
     @param inListener the listener object to remove.
     */
    void removeDeleteButtonListener(Button::Listener* inListener);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(BlomeFileFilterRuleViewBase)
    
protected:
    SampleLibrary& linkedSampleLibrary;
    SampleFileFilterRuleBase& linkedFilterRule;
    std::unique_ptr<ComboBox> mCompareOperatorChooser;
    std::unique_ptr<ToggleButton> mActivateRuleButton;
    std::unique_ptr<TextButton> mDeleteRuleButton;
    int deleteButtonWidth = 50;
};
