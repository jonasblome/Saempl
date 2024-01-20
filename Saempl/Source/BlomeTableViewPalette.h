/*
 ==============================================================================
 
 BlomeTableViewPalette.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "BlomeTableViewBase.h"

/**
 Implements BlomeTableViewBase for the SamplePalettePanel.
 
 Handles removing of each row's SampleItem from the SampleLibrary::mSampleItemsPalette.
 */
class BlomeTableViewPalette
:
public BlomeTableViewBase
{
public:
    /**
     Constructor for the table view on the SampleLibrary::mPaletteSampleItems collection.
     
     @param inSampleLibrary the sample library of the current plugin instance.
     @param inSampleItemPanel the panel for previewing SampleItem objects.
     */
    BlomeTableViewPalette(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~BlomeTableViewPalette();
    
private:
    void cellClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent) override;
    void filesDropped(StringArray const & files, int x, int y) override;
    /**
     Removes the SampleItem object of the selected rows from the SampleLibrary::mPaletteSampleItems collection.
     */
    void removeSampleItemFromPalette();
};
