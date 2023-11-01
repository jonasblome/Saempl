/*
  ==============================================================================

    BlomeTableViewNavigation.h
    Created: 1 Nov 2023 10:34:42am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "BlomeTableViewBase.h"

class BlomeTableViewNavigation
:   public BlomeTableViewBase
{
public:
    // Constructors
    BlomeTableViewNavigation(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel);
    ~BlomeTableViewNavigation();
    
    // Methods
    void cellClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent) override;
    void filesDropped(StringArray const & files, int x, int y) override;
    void deleteFile(bool deletePermanently);
    void sortOrderChanged(int newSortColumnId, bool isForwards) override;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(BlomeTableViewNavigation)
    
    // Fields
    
    // Methods
    
};
