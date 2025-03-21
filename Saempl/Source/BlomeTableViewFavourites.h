/*
 ==============================================================================
 
 BlomeTableViewFavourites.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "BlomeTableViewBase.h"
#include "SampleNavigationPanel.h"

/**
 Implements BlomeTableViewBase for the SampleFavouritesPanel.
 
 Handles removing of each row's SampleItem from the sample favourites collection.
 */
class BlomeTableViewFavourites
:
public BlomeTableViewBase
{
public:
    /**
     Constructor for the table view on the SampleLibrary::mFavouriteSampleItems collection.
     
     @param inProcessor the processor object of the current plugin instance.
     @param inSampleItemPanel the panel for previewing SampleItem objects.
     */
    BlomeTableViewFavourites(SaemplAudioProcessor& inProcessor,
                             SampleNavigationPanel & inSampleNavigationPanel,
                             SampleItemPanel& inSampleItemPanel,
                             AudioPlayer& inAudioPlayer);
    ~BlomeTableViewFavourites();
    
private:
    SampleNavigationPanel & sampleNavigationPanel;
    void cellClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent) override;
    void filesDropped(StringArray const & files, int x, int y) override;
    /**
     Removes the SampleItem object of the selected rows from the sample favourites collection collection.
     */
    void removeSampleItemFromFavourites();
    /**
     Opens the grid view at the location of the chosen sample.
     */
    void showSampleInNavigation();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlomeTableViewFavourites);
};
