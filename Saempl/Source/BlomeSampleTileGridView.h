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
    /**
     Sorts the grid by sample item similarity.
     */
    void sortGrid();
    /**
     Sets the grids' boundaries and performs the layout of the grid's components.
     */
    void performGridLayout(float inZoomFactor);
    Point<int> selectRandomTile();
    
private:
    SampleLibrary& sampleLibrary;
    BlomeStyling::Ptr style;
    std::unique_ptr<Grid> mSampleTileGrid;
    SampleItemPanel& sampleItemPanel;
    OwnedArray<BlomeSampleItemTileView> mSampleItemTiles;
    Array<int> mSelectedSampleTileIndices;
    StringArray mAddedSampleFilePaths;
    std::unique_ptr<SampleItemGridSorter> mGridSorter;
    bool sampleItemCollectionChanged;
    int optimalWidth;
    int optimalHeight;
    float currentZoomFactor;
    
    /**
     Creates tile components for each sample item and adds them to a square grid.
     */
    void setupGrid();
    void paint(Graphics& g) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void filesDropped(StringArray const & files, int x, int y) override;
    bool isInterestedInFileDrag(StringArray const & files) override;
    void showPopupMenu();
    void mouseUp(MouseEvent const & event) override;
    void deleteFiles(bool deletePermanently);
    void deselectAll();
    void addToPalette();
    void reanalyseSamples();
};
