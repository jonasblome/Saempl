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
     
     @param inSampleLibrary the library object of the current plugin instance.
     @param inSampleItemPanel the panel for previewing SampleItem objects.
     */
    SampleFavouritesPanel(SaemplAudioProcessor & inProcessor,
                          SampleNavigationPanel & inSampleNavigationPanel,
                          SampleItemPanel & inSampleItemPanel,
                          AudioPlayer& inAudioPlayer);
    ~SampleFavouritesPanel();
    bool keyPressed(KeyPress const & key) override;
    
private:
    SampleLibrary & sampleLibrary;
    SampleItemPanel & sampleItemPanel;
    SampleNavigationPanel & sampleNavigationPanel;
    AudioPlayer& audioPlayer;
    std::unique_ptr<BlomeTableViewFavourites> mSampleTable;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleFavouritesPanel);
};
