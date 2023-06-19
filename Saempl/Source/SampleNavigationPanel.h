/*
  ==============================================================================

    SampleItemNavigationPanel.h
    Created: 18 Jun 2023 2:07:02pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleTablePanel.h"
#include "SampleFilterPanel.h"

class SampleNavigationPanel
:   public PanelBase
{
public:
    // Constructors
    SampleNavigationPanel(SaemplAudioProcessor &inProcessor);
    ~SampleNavigationPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
    void toggleSampleItemFilterPanel();
    
private:
    // Fields
    SaemplAudioProcessor& currentProcessor;
    std::unique_ptr<SampleFilterPanel> mSampleItemFilterPanel;
    std::unique_ptr<SampleTablePanel> mSampleItemTablePanel;
    
    // Methods
    
};
