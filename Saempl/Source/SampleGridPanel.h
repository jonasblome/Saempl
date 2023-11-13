/*
  ==============================================================================

    SampleGridPanel.h
    Created: 13 Nov 2023 5:04:23pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleItemPanel.h"
#include "BlomeSampleItemTileView.h"

/**
 Holds the table for displaying all \ref SampleLibrary::mAllSampleItems
 
 Implements:
 \ref PanelBase
 */
class SampleGridPanel
:
public PanelBase
{
public:
    /**
     The constructor for the sample table panel.
     
     @param inSampleLibrary the library object of the current plugin instance.
     @param inSampleItemPanel the panel for previewing \ref SampleItem objects.
     */
    SampleGridPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~SampleGridPanel();
    
private:
    SampleLibrary& sampleLibrary;
    SampleItemPanel& linkedSampleItemPanel;
    BlomeStyling::Ptr style;
    std::unique_ptr<Grid> mSampleTileGrid;
    OwnedArray<BlomeSampleItemTileView> mSampleItemTiles;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
};
