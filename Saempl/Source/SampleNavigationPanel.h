/*
  ==============================================================================

    SampleNavigationPanel.h
    Created: 24 Jun 2023 4:28:16pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleLibraryPanel.h"
#include "SampleTablePanel.h"
#include "SampleItemPanel.h"

class SampleNavigationPanel
:   public PanelBase
{
public:
    // Constructors
    SampleNavigationPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~SampleNavigationPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
    void showNavigationPanel(int inNavigationType);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleNavigationPanel)
    
    // Fields
    SampleItemPanel& linkedSampleItemPanel;
    std::unique_ptr<SampleLibraryPanel> mSampleLibraryPanel;
    std::unique_ptr<SampleTablePanel> mSampleTablePanel;
    SampleLibrary& sampleLibrary;
    
    // Methods
    
};
