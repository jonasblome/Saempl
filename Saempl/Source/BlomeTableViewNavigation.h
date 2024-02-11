/*
 ==============================================================================
 
 BlomeTableViewNavigation.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "BlomeTableViewBase.h"

/**
 Implements BlomeTableViewBase for the SampleTablePanel.
 
 Handles sorting and deleting of each row's SampleItem.
 */
class BlomeTableViewNavigation
:
public BlomeTableViewBase,
public ChangeListener
{
public:
    /**
     Constructor for the table view on the SampleLibrary::mFilteredSampleItems collection.
     
     @param inSampleLibrary the sample library of the current plugin instance.
     @param inSampleItemPanel the panel for previewing SampleItem objects.
     */
    BlomeTableViewNavigation(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~BlomeTableViewNavigation();
    /**
     Resorts the table with the currently chosen column and direction.
     */
    void resortTable();
    
private:
    void cellClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent) override;
    void filesDropped(StringArray const & files, int x, int y) override;
    /**
     Deletes all files of the rows that are currently selected.
     
     @param deletePermanently deletes files permanently if true, move them to the trash if false.
     */
    void deleteFiles(bool deletePermanently);
    /**
     Adds all selected files to the palette sample items collection.
     */
    void addToPalette();
    void changeListenerCallback(ChangeBroadcaster* source) override;
};
