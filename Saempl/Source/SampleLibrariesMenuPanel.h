/*
  ==============================================================================

    SampleItemLibraryListMenuPanel.h
    Created: 18 Jun 2023 6:29:42pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleLibrariesPanel.h"

class SampleLibrariesMenuPanel
:   public PanelBase
{
public:
    // Constructors
    SampleLibrariesMenuPanel(SaemplAudioProcessor& inProcessor, SampleLibrariesPanel& inSampleItemLibraryListPanel);
    ~SampleLibrariesMenuPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
private:
    // Fields
    SampleLibrariesPanel& linkedSampleItemLibraryListPanel;
    std::unique_ptr<TextButton> mRefreshSampleDatabaseButton;
    
    // Methods
    
};
