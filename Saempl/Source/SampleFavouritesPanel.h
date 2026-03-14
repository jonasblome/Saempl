/*
 ==============================================================================
 
 SampleFavouritesPanel.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "PanelBase.h"
#include "BlomeTableViewFavourites.h"
#include "SampleNavigationPanel.h"

/**
 Holds the table for displaying the sample favourites collection.
 */
class SampleFavouritesPanel
:
public PanelBase
{
public:
    /**
     The constructor for the sample favourites panel.
     
     @param inProcessor the processor object of the current plugin instance.
     @param inSampleNavigationPanel the panel for navigating the samples.
     @param inSampleItemPanel the panel for previewing SampleItem objects.
     @param inAudioPlayer the audio player for sample preview playback.
     */
    SampleFavouritesPanel(SaemplAudioProcessor& inProcessor,
                          SampleNavigationPanel& inSampleNavigationPanel,
                          SampleItemPanel& inSampleItemPanel,
                          AudioPlayer& inAudioPlayer);
    ~SampleFavouritesPanel();
    bool keyPressed(KeyPress const& key) override;
    
private:
    SampleLibrary& sampleLibrary;
    SampleItemPanel& sampleItemPanel;
    SampleNavigationPanel& sampleNavigationPanel;
    AudioPlayer& audioPlayer;
    std::unique_ptr<BlomeTableViewFavourites> mSampleTable;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleFavouritesPanel);
};
