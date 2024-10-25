/*
 ==============================================================================
 
 SampleGridPanel.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "PanelBase.h"
#include "SampleItemPanel.h"
#include "BlomeSampleGridView.h"
#include "SampleGridOptionsPanel.h"

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
    SampleGridPanel(SaemplAudioProcessor& inProcessor,
                    SampleItemPanel& inSampleItemPanel,
                    AudioPlayer& inAudioPlayer);
    ~SampleGridPanel();
    /**
     Selects a random sample on the grid.
     */
    void selectRandomSample();
    bool keyPressed(KeyPress const & key) override;
    /**
     Show the sample in the grid view.
     
     @param inFilePath the path to the sample file.
     */
    void showSample(String inFilePath);
    
private:
    SampleLibrary& sampleLibrary;
    SampleItemPanel& sampleItemPanel;
    AudioPlayer& audioPlayer;
    std::unique_ptr<BlomeSampleGridView> mSampleGrid;
    std::unique_ptr<Viewport> mGridViewport;
    std::unique_ptr<BlomeImageButton> mChangeGridClusteringOptionsButton;
    std::unique_ptr<Slider> mZoomSlider;
    std::unique_ptr<TextButton> mHarmonicPresetButton;
    std::unique_ptr<TextButton> mDrumsPresetButton;
    std::unique_ptr<TextButton> mMonophonicPresetButton;
    std::unique_ptr<TextButton> mFoleyPresetButton;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
    void visibilityChanged() override;
    /**
     Moves the viewport's centre position the given position.
     
     @param newPosition the new centre position.
     */
    void centrePositionInGridViewport(Point<int>& newPosition);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleGridPanel);
};
