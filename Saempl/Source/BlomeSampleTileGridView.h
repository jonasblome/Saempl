/*
 ==============================================================================
 
 BlomeSampleTileGridView.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "BlomeStyleDefinitions.h"
#include "SampleLibrary.h"
#include "BlomeSampleItemTileView.h"
#include "SampleItemGridSorter.h"

class BlomeSampleTileGridView
:
public Component,
public ChangeListener,
public FileDragAndDropTarget
{
public:
    BlomeSampleTileGridView(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel);
    ~BlomeSampleTileGridView();
    
private:
    SampleLibrary& sampleLibrary;
    BlomeStyling::Ptr style;
    std::unique_ptr<Grid> mSampleTileGrid;
    SampleItemPanel& sampleItemPanel;
    OwnedArray<BlomeSampleItemTileView> mSampleItemTiles;
    StringArray mAddedSampleFilePaths;
    SampleItemGridSorter mGridSorter;
    
    void paint(Graphics& g) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    /**
     Creates tile components for each sample item and adds them to a square grid.
     */
    void setupGrid();
    void filesDropped(StringArray const & files, int x, int y) override;
    bool isInterestedInFileDrag(StringArray const & files) override;
};
