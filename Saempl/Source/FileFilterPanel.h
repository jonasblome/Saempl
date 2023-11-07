/*
  ==============================================================================

    FileFilterPanel.h
    Created: 29 Oct 2023 12:14:04pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "BlomeFileFilterRuleViewBase.h"
#include "BlomeFileFilterRuleViewTitle.h"
#include "BlomeFileFilterRuleViewLength.h"
#include "NoiseOverlayPanel.h"

/**
 Holds the \ref BlomeFileFilterRuleViewBase components.
 
 Implements:
 \ref PanelBase
 \ref juce::TextButton::Listener
 Has dropdown and button to add new filter rules of different types.
 */
class FileFilterPanel
:   public PanelBase,
    public TextButton::Listener
{
public:
    /**
     The constructor of the panel for the library's file filter.
     
     @param inSampleLibrary the library object of the current plugin instance.
     */
    FileFilterPanel(SaemplAudioProcessor& inProcessor);
    ~FileFilterPanel();
    
private:
    SampleLibrary& sampleLibrary;
    SampleFileFilter& libraryFileFilter;
    std::unique_ptr<ComboBox> mNewRuleTypeChooser;
    std::unique_ptr<BlomeImageButton> mAddFilterRuleButton;
    OwnedArray<BlomeFileFilterRuleViewBase> mFilterRuleViews;
    
    void paint(Graphics& g) override;
    /**
     Generates a \ref BlomeFileFilterRuleViewBase for the given rule and adds it to the \ref FileFilterPanel::mFilterRuleViews collection.
     
     @param rule the rule for which to create the view.
     */
    void generateRuleView(SampleFileFilterRuleBase* rule);
    /**
     Adds a new rule to the \ref SampleFileFilter and adds a view to the panel.
     */
    void addFilterRuleView();
    /**
     Removes a rule from the \ref SampleFileFilter and its respective view.
     */
    void removeFilterRule(SampleFileFilterRuleBase const & inFilterRule);
    void setPanelComponents() override;
    void buttonClicked(Button* button) override;
};
