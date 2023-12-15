/*
 ==============================================================================
 
 BlomeTableViewPalette.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "BlomeTableViewBase.h"

/**
 Implements \ref BlomeTableViewBase for the \ref SamplePalettePanel.
 
 Implements:
 \ref BlomeTableViewBase
 Handles removing of each row's \ref SampleItem from the \ref SampleLibrary::mSampleItemsPalette.
 */
class BlomeTableViewPalette
:
public BlomeTableViewBase
{
public:
    /**
     Constructor for the table view on the \ref SampleLibrary::mPaletteSampleItems collection.
     
     @param inSampleLibrary the sample library of the current plugin instance.
     @param inSampleItemPanel the panel for previewing \ref SampleItem objects.
     */
    BlomeTableViewPalette(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~BlomeTableViewPalette();
    
private:
    void cellClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent) override;
    void filesDropped(StringArray const & files, int x, int y) override;
    /**
     Removes the \ref SampleItem object of the selected rows from the \ref SampleLibrary::mPaletteSampleItems collection.
     */
    void removeSampleItemFromPalette();
};
