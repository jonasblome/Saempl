/*
  ==============================================================================

    SamplePalettePanel.h
    Created: 31 Oct 2023 3:49:49pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "BlomeTableViewPalette.h"

class SamplePalettePanel
:   public PanelBase
{
public:
    // Constructors
    SamplePalettePanel(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel);
    ~SamplePalettePanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SamplePalettePanel)
    
    // Fields
    SampleLibrary& sampleLibrary;
    SampleItemPanel& linkedSampleItemPanel;
    std::unique_ptr<BlomeTableViewPalette> mSampleTable;
    
    // Methods
    
};
