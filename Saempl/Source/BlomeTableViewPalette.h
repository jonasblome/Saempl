/*
  ==============================================================================

    BlomeTableViewPalette.h
    Created: 1 Nov 2023 10:34:25am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "BlomeTableViewBase.h"

class BlomeTableViewPalette
:   public BlomeTableViewBase
{
public:
    // Constructors
    BlomeTableViewPalette(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel);
    ~BlomeTableViewPalette();
    
    // Methods
    void cellClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent) override;
    void filesDropped(StringArray const & files, int x, int y) override;
    void removeSampleItemFromPalette();
    void sortOrderChanged(int newSortColumnId, bool isForwards) override;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(BlomeTableViewPalette)
    
    // Fields
    
    // Methods
    
};
