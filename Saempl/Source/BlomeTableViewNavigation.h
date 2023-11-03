/*
  ==============================================================================

    BlomeTableViewNavigation.h
    Created: 1 Nov 2023 10:34:42am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "BlomeTableViewBase.h"

/**
 Implements \ref BlomeTableViewBase for the \ref SampleTablePanel.
 
 Implements:
 \ref BlomeTableViewBase
 Handles sorting and deleting of each row's \ref SampleItem.
 */
class BlomeTableViewNavigation
:   public BlomeTableViewBase
{
public:
    /**
     Constructor for the table view on the \ref SampleLibrary::mFilteredSampleItems collection.
     
     @param inSampleLibrary the sample library of the current plugin instance.
     @param inSampleItemPanel the panel for previewing \ref SampleItem objects.
     */
    BlomeTableViewNavigation(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel);
    ~BlomeTableViewNavigation();
    void cellClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent) override;
    void filesDropped(StringArray const & files, int x, int y) override;
    /**
     Deletes all files of the rows that are currently selected.
     
     @param deletePermanently deletes files permanently if true, move them to the trash if false.
     */
    void deleteFile(bool deletePermanently);
    void sortOrderChanged(int newSortColumnId, bool isForwards) override;
    
private:
    // JUCE_HEAVYWEIGHT_LEAK_DETECTOR(BlomeTableViewNavigation)
};
