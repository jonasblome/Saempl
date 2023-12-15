/*
 ==============================================================================
 
 BlomeSampleTileGridView.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeSampleTileGridView.h"

BlomeSampleTileGridView::BlomeSampleTileGridView(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel)
:
sampleLibrary(inSampleLibrary),
sampleItemPanel(inSampleItemPanel)
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
    // Setup tile grid
    mSampleTileGrid = std::make_unique<Grid>();
    mSampleTileGrid->setGap(Grid::Px(style->PANEL_MARGIN * 0.5f));
    mSampleTileGrid->autoFlow = Grid::AutoFlow::row;
    using Track = Grid::TrackInfo;
    mSampleTileGrid->autoRows = Track(1_fr);
    mSampleTileGrid->autoColumns = Track(1_fr);
    
    mSampleItemTiles.clear();
    
    for (SampleItem* sample : sampleLibrary.getSampleItems(FILTERED_SAMPLES))
    {
        addAndMakeVisible(mSampleItemTiles.add(new BlomeSampleItemTileView(sample, sampleLibrary, sampleItemPanel)));
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
}

bool BlomeSampleTileGridView::isInterestedInFileDrag(StringArray const & files)
{
    return true;
}
