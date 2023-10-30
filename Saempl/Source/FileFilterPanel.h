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

class FileFilterPanel
:   public PanelBase
{
public:
    // Constructors
    FileFilterPanel(SampleLibrary& inSampleLibrary);
    ~FileFilterPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(FileFilterPanel)
    
    // Fields
    SampleFileFilter& libraryFileFilter;
    SampleLibrary& linkedLibrary;
    std::unique_ptr<ComboBox> mNewRuleTypeChooser;
    std::unique_ptr<TextButton> mAddFilterRuleButton;
    OwnedArray<BlomeFileFilterRuleViewBase> mFilterRuleViews;
    
    // Methods
    
};
