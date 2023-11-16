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

/**
 Holds the table for displaying the \ref SampleLibrary::mPaletteSampleItems
 
 Implements:
 \ref PanelBase
 */
class SamplePalettePanel
:
public PanelBase
{
public:
    /**
     The constructor for the sample palette panel.
     
     @param inSampleLibrary the library object of the current plugin instance.
     @param inSampleItemPanel the panel for previewing \ref SampleItem objects.
     */
    SamplePalettePanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~SamplePalettePanel();
    
private:
    SampleLibrary& sampleLibrary;
    SampleItemPanel& sampleItemPanel;
    std::unique_ptr<BlomeTableViewPalette> mSampleTable;
    BlomeStyling::Ptr style;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
};
