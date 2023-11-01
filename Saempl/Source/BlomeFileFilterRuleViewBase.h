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

class BlomeFileFilterRuleViewBase
:   public Component,
    public ComboBox::Listener
{
public:
    // Constructors
    BlomeFileFilterRuleViewBase(SampleFileFilterRuleBase& inFilterRule, SampleLibrary& inSampleLibrary);
    virtual ~BlomeFileFilterRuleViewBase();
    
    // Methods
    void paint(Graphics& g) override;
    virtual void setPanelComponents();
    void resized() override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    SampleFileFilterRuleBase& getLinkedFilterRule();
    void addDeleteButtonListener(Button::Listener* inListener);
    void removeDeleteButtonListener(Button::Listener* inListener);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(BlomeFileFilterRuleViewBase)
    
protected:
    // Fields
    SampleLibrary& linkedSampleLibrary;
    SampleFileFilterRuleBase& linkedFilterRule;
    std::unique_ptr<ComboBox> mCompareOperatorChooser;
    std::unique_ptr<ToggleButton> mActivateRuleButton;
    std::unique_ptr<TextButton> mDeleteRuleButton;
    int deleteButtonWidth = 50;
    
    // Methods
    
    
};
