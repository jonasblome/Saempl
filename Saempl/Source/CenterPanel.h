/*
  ==============================================================================

    CenterPanel.h
    Created: 27 May 2023 9:07:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleLibrariesPanel.h"
#include "SampleItemPanel.h"

class CenterPanel
:   public PanelBase
{
public:
    // Constructors
    CenterPanel(SaemplAudioProcessor& inProcessor);
    ~CenterPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
private:
    // Fields
    SaemplAudioProcessor& currentProcessor;
    std::unique_ptr<SampleLibrariesPanel> mSampleDatabaseTablePanel;
    std::unique_ptr<SampleItemPanel> mSampleItemPanel;
    std::unique_ptr<ToggleButton> mTogglePanelButton;
    
    // Methods
    void toggleSampleItemPanel();
    
};
