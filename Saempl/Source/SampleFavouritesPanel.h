/*
 ==============================================================================
 
 SampleFavouritesPanel.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "PanelBase.h"
#include "BlomeTableViewFavourites.h"

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
    SampleFavouritesPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~SampleFavouritesPanel();
    
private:
    SampleLibrary& sampleLibrary;
    SampleItemPanel& sampleItemPanel;
    std::unique_ptr<BlomeTableViewFavourites> mSampleTable;
    BlomeStyling::Ptr style;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleFavouritesPanel);
};
