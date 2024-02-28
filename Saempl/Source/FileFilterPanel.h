/*
 ==============================================================================
 
 FileFilterPanel.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "PanelBase.h"
#include "BlomeFileFilterRuleViewBase.h"
#include "BlomeFileFilterRuleViewTitle.h"
#include "BlomeFileFilterRuleViewLength.h"
#include "BlomeFileFilterRuleViewLoudnessDecibel.h"
#include "BlomeFileFilterRuleViewLoudnessLUFS.h"
#include "BlomeFileFilterRuleViewTempo.h"
#include "BlomeFileFilterRuleViewKey.h"

/**
 Holds the BlomeFileFilterRuleViewBase components.
 
 Has a dropdown menu and button to add new filter rules of different types.
 */
class FileFilterPanel
:
public PanelBase,
public TextButton::Listener
{
public:
    /**
     The constructor of the panel for the library's file filter.
     
     @param inProcessor the processor object of the current plugin instance.
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
     Generates a BlomeFileFilterRuleViewBase for the given rule and adds it to the FileFilterPanel::mFilterRuleViews collection.
     
     @param rule the rule for which to create the view.
     */
    void generateRuleView(SampleFileFilterRuleBase* rule);
    /**
     Adds a new rule to the SampleFileFilter and adds a view to the panel.
     */
    void addFilterRuleView();
    /**
     Removes a rule from the SampleFileFilter and its respective view.
     
     @param inFilterRule the filter rule to remove.
     */
    void removeFilterRule(SampleFileFilterRuleBase const & inFilterRule);
    void setAddFilterRuleButton();
    void setNewRuleTypeChooser();
    void setPanelComponents() override;
    void buttonClicked(Button* button) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileFilterPanel);
};
