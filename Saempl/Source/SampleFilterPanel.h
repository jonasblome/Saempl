/*
  ==============================================================================

    SampleItemFilterPanel.h
    Created: 17 Jun 2023 1:19:35pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleFilterCondition.h"
#include "FilterConditionView.h"
#include "BlomeStyleDefinitions.h"

class SampleFilterPanel
:   public PanelBase
{
public:
    // Constructors
    SampleFilterPanel(SaemplAudioProcessor& inProcessor);
    ~SampleFilterPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
private:
    // Fields
    std::unique_ptr<TextButton> mAddFilterConditionButton;
    OwnedArray<FilterConditionView> mFilterConditionViews;
    OwnedArray<SampleFilterCondition>* filterConditions;
    
    // Methods
    void setFilterConditionViews();
    void addFilterCondition();
    
};
