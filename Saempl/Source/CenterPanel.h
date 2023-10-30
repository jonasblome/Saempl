/*
  ==============================================================================

    CenterPanel.h
    Created: 27 May 2023 9:07:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleNavigationPanel.h"
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
    void showNavigationPanel(NavigationPanelType inPanelType);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(CenterPanel)
    
    // Fields
    SaemplAudioProcessor& currentProcessor;
    std::unique_ptr<SampleNavigationPanel> mSampleNavigationPanel;
    std::unique_ptr<SampleItemPanel> mSampleItemPanel;
    std::unique_ptr<ToggleButton> mToggleSampleItemPanelButton;
    
    // Methods
    void toggleSampleItemPanel();
    
};
