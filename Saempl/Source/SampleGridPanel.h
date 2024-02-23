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
 Holds the grid for displaying the library sample item collection.
 */
class SampleGridPanel
:
public PanelBase
{
public:
    /**
     The constructor for the sample table panel.
     
     @param inSampleLibrary the library object of the current plugin instance.
     @param inSampleItemPanel the panel for previewing SampleItem objects.
     */
    SampleGridPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~SampleGridPanel();
    /**
     Selects a random sample on the grid.
     */
    void selectRandomSample();
    
private:
    SampleLibrary& sampleLibrary;
    SampleItemPanel& sampleItemPanel;
    BlomeStyling::Ptr style;
    std::unique_ptr<BlomeSampleTileGridView> mSampleTileGrid;
    std::unique_ptr<Viewport> mGridViewport;
    std::unique_ptr<Slider> mZoomSlider;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
    void visibilityChanged() override;
    bool keyPressed(KeyPress const & key) override;
    void mouseMagnify(MouseEvent const & mouseEvent, float magnifyAmount) override;
    /**
     Moves the viewport's centre position the given position.
     
     @param newPosition the new centre position.
     */
    void centrePositionInGridViewport(Point<int>& newPosition);
};
