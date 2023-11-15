/*
 ==============================================================================
 
 BlomeSampleTileGridView.cpp
 Created: 14 Nov 2023 1:29:30pm
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeSampleTileGridView.h"

BlomeSampleTileGridView::BlomeSampleTileGridView(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel)
:
sampleLibrary(inSampleLibrary),
linkedSampleItemPanel(inSampleItemPanel)
{
    sampleLibrary.addChangeListener(this);
    setupGrid();
}

BlomeSampleTileGridView::~BlomeSampleTileGridView()
{
    sampleLibrary.removeChangeListener(this);
}

void BlomeSampleTileGridView::paint(Graphics& g)
{
    
}

void BlomeSampleTileGridView::setupGrid()
{
//    mSampleItemTiles.clear();
//    
//    for (SampleItem* sample : sampleLibrary.getSampleItems(FILTERED_SAMPLES))
//    {
//        addAndMakeVisible(mSampleItemTiles.add(new BlomeSampleItemTileView(*sample, sampleLibrary, linkedSampleItemPanel)));
//    }
    
    // Setup tile grid
    mSampleTileGrid = std::make_unique<Grid>();
    mSampleTileGrid->setGap(Grid::Px(style->PANEL_MARGIN * 0.5f));
    mSampleTileGrid->autoFlow = Grid::AutoFlow::column;
    using Track = Grid::TrackInfo;
    mSampleTileGrid->autoRows = Track(1_fr);
    mSampleTileGrid->autoColumns = Track(1_fr);
    
    // Go through all tiles and check if corresponding
    // sample items exist, and if not delete them
    OwnedArray<BlomeSampleItemTileView> tilesToDelete;
    
    if (mAddedSampleFilePaths.size() != sampleLibrary.getFilteredFilePaths().size())
    {
        for (BlomeSampleItemTileView* tile : mSampleItemTiles)
        {
            String tileFilePath = tile->getSampleItemFilePath().convertToPrecomposedUnicode();
            
            if (!sampleLibrary.getFilteredFilePaths().contains(tileFilePath))
            {
                tilesToDelete.add(tile);
                mAddedSampleFilePaths.removeString(tileFilePath);
            }
        }
    }
    
    for (BlomeSampleItemTileView* tile : tilesToDelete)
    {
        mSampleItemTiles.removeObject(tile);
    }
    
    tilesToDelete.clear(false);
    
    // Add tiles for each new sample item
    if (mAddedSampleFilePaths.size() != sampleLibrary.getFilteredFilePaths().size())
    {
        for (SampleItem* sample: sampleLibrary.getSampleItems(FILTERED_SAMPLES))
        {
            if (!mAddedSampleFilePaths.contains(sample->getFilePath()))
            {
                addAndMakeVisible(mSampleItemTiles.add(new BlomeSampleItemTileView(sample, sampleLibrary, linkedSampleItemPanel)));
            }
            
            mAddedSampleFilePaths.add(sample->getFilePath().convertToPrecomposedUnicode());
        }
    }
    
    mSampleTileGrid->templateRows = Array<Track>();
    for (int i = 0; i < sqrt(mSampleItemTiles.size()); i++)
    {
        mSampleTileGrid->templateRows.add(Track(1_fr));
    }
    mSampleTileGrid->templateColumns = Array<Track>();
    for (int i = 0; i < sqrt(mSampleItemTiles.size()); i++)
    {
        mSampleTileGrid->templateColumns.add(Track(1_fr));
    }
    mSampleTileGrid->items.addArray(mSampleItemTiles);
    setBounds(0,
              0,
              sqrt(mSampleItemTiles.size()) * 100 + style->PANEL_MARGIN * 0.5,
              sqrt(mSampleItemTiles.size()) * 100 + style->PANEL_MARGIN * 0.5);
    mSampleTileGrid->performLayout(Rectangle<int>(0,
                                                  0,
                                                  sqrt(mSampleItemTiles.size()) * 100,
                                                  sqrt(mSampleItemTiles.size()) * 100));
}

void BlomeSampleTileGridView::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == &sampleLibrary)
    {
        setupGrid();
    }
}

void BlomeSampleTileGridView::filesDropped(StringArray const & files, int x, int y)
{
    // Adding all the dropped files to the database
    for(int f = 0; f < files.size(); f++)
    {
        sampleLibrary.addAllToSampleItems(files[f]);
    }
    
    sampleLibrary.refresh();
}

bool BlomeSampleTileGridView::isInterestedInFileDrag(StringArray const & files)
{
    return true;
}
