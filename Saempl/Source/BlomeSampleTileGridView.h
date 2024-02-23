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
#include "SampleItemGridClusterer.h"

/**
 The view class for the sample grid.
 
 This handles clustering and arranging the samples as tiles on a grid.
 */
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
     Clusters the grid by sample item similarity.
     */
    void clusterGrid();
    /**
     Sets the grids' boundaries and performs the layout of the grid's components.
     */
    void performGridLayout();
    /**
     Sets the zoom factor of the grid tiles and tile gap.
     
     @param inZoomFactor the zoom factor from 0.0 to 1.0.
     */
    void setZoomFactor(float inZoomFactor);
    /**
     Selects a random tile in the grid and returns its position.
     
     @returns the center point of the selected tile.
     */
    Point<int> selectRandomTile();
    /**
     Loads the first of all selected tiles into the audio player and plays it back.
     */
    void loadSelectedTileIntoAudioPlayer();
    /**
     Selects all tiles on the grid.
     */
    void selectAll();
    /**
     Deselects all tiles on the grid.
     */
    void deselectAll();
    /**
     Selects the tile to the left of the currrently selected tile.
     
     Selects the first tile if none are currently selected.
     
     @returns the center position of the newly selected tile.
     */
    Point<int> selectLeft();
    
    /**
     Selects the tile above the currrently selected tile.
     
     Selects the first tile if none are currently selected.
     
     @returns the center position of the newly selected tile.
     */
    Point<int> selectUp();
    
    /**
     Selects the tile to the right of the currrently selected tile.
     
     Selects the first tile if none are currently selected.
     
     @returns the center position of the newly selected tile.
     */
    Point<int> selectRight();
    
    /**
     Selects the tile below the currrently selected tile.
     
     Selects the first tile if none are currently selected.
     
     @returns the center position of the newly selected tile.
     */
    Point<int> selectDown();
    /**
     Marks the grid as ready for clustering if something happened that requires re-clustering.
     */
    void setReadyForClustering();
    /**
     @returns the ratio between the min. and max. tile size.
     */
    float getTileMinMaxRelation();
    /**
     Starts playback of the first of all selected samples.
     */
    void playSelectedSample();
    /**
     Stops playback of the currently playing sample.
     */
    void stopSelectedSample();
    
private:
    SampleLibrary& sampleLibrary;
    std::unique_ptr<AudioPlayer> mAudioPlayer;
    BlomeStyling::Ptr style;
    std::unique_ptr<Grid> mSampleTileGrid;
    SampleItemPanel& sampleItemPanel;
    OwnedArray<BlomeSampleItemTileView> mSampleItemTiles;
    Array<int> mSelectedSampleTileIndices;
    OwnedArray<SampleItem> emptySquares;
    StringArray mAddedSampleFilePaths;
    std::unique_ptr<SampleItemGridClusterer> mGridClusterer;
    bool sampleItemCollectionChanged;
    int optimalWidth;
    int optimalHeight;
    float currentZoomFactor;
    int minTileWidth = 85;
    int maxTileWidth = 150;
    
    /**
     Creates tile components for each sample item and adds them to a square grid.
     */
    void setupGrid();
    void paint(Graphics& g) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void filesDropped(StringArray const & files, int x, int y) override;
    bool isInterestedInFileDrag(StringArray const & files) override;
    void mouseDrag(MouseEvent const & mouseEvent) override;
    /**
     Shows a popup menu with options to alter the selected sample items.
     */
    void showPopupMenu();
    /**
     Selects a tile in the grid and returns it.
     
     @param inTileIndex the index of the tile in the tile collection.
     
     @returns a pointer to the selected tile view object.
     */
    BlomeSampleItemTileView* selectTile(int inTileIndex);
    /**
     Deselects a tile in the grid.
     
     @param inTileIndex the index of the tile in the tile collection
     */
    void deselectTile(int inTileIndex);
    void mouseUp(MouseEvent const & event) override;
    /**
     Deletes the files and sample items of the selected tiles.
     
     @param deletePermanently deletes files permanently if true, move them to the trash if false.
     */
    void deleteFiles(bool deletePermanently);
    /**
     Adds all selected samples to the library sample favourites collection.
     */
    void addToFavourites();
    /**
     Starts the analysis process for all selected samples.
     */
    void reanalyseSamples();
    /**
     Returns the center point of a given tile.
     
     @param inTile a pointer to the tile.
     
     @returns the point of the tile's center.
     */
    Point<int> getTileCenter(BlomeSampleItemTileView* inTile);
};
