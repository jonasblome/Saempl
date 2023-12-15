/*
  ==============================================================================

    SampleGridPanel.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleItemPanel.h"
#include "BlomeSampleTileGridView.h"

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
    SampleItemPanel& sampleItemPanel;
    BlomeStyling::Ptr style;
    std::unique_ptr<BlomeSampleTileGridView> mSampleTileGrid;
    std::unique_ptr<Viewport> mGridViewport;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
};
