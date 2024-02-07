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
    // Retrieve sample items
    OwnedArray<SampleItem>& sampleItems = sampleLibrary.getSampleItems(FILTERED_SAMPLES);
    
    // Setup tile grid
    mSampleTileGrid = std::make_unique<Grid>();
    mSampleTileGrid->setGap(Grid::Px(style->PANEL_MARGIN * 0.5f));
    mSampleTileGrid->autoFlow = Grid::AutoFlow::row;
    using Track = Grid::TrackInfo;
    mSampleTileGrid->autoRows = Track(1_fr);
    mSampleTileGrid->autoColumns = Track(1_fr);
    
    // Calculate optimal rectangle side lengths to minimise empty tiles,
    // while still trying to approximate a square grid
    int numTiles = sampleItems.size();
    int numItemsSqrt = std::ceil(sqrt(numTiles));
    int optimalNumEmptySquares = numItemsSqrt * numItemsSqrt - numTiles;
    int optimalWidth = numItemsSqrt;
    int optimalHeight = numItemsSqrt;
    int maximumLengthDeviation = 30;
    
    if (optimalNumEmptySquares != 0)
    {
        int currentMaximumLengthDeviation = jmin<int>(maximumLengthDeviation, std::ceil(numItemsSqrt * 0.6));
        
        for (int i = 1; i <= currentMaximumLengthDeviation; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                int numEmptySquares = (numItemsSqrt - i) * (numItemsSqrt + j) - numTiles;
                
                if (numEmptySquares < 0)
                {
                    // Current ratio is too small to fit all items
                    break;
                }
                
                if (numEmptySquares < optimalNumEmptySquares)
                {
                    optimalNumEmptySquares = numEmptySquares;
                    optimalHeight = numItemsSqrt - i;
                    optimalWidth = numItemsSqrt + j;
                }
            }
        }
    }
    
    // Set up grid rows
    mSampleTileGrid->templateRows = Array<Track>();
    for (int i = 0; i < optimalHeight; i++)
    {
        mSampleTileGrid->templateRows.add(Track(1_fr));
    }
    
    // Set up grid columns
    mSampleTileGrid->templateColumns = Array<Track>();
    for (int i = 0; i < optimalWidth; i++)
    {
        mSampleTileGrid->templateColumns.add(Track(1_fr));
    }
    
    // Add empty squares to sample items
    Array<SampleItem*> emptySquares;
    for (int e = 0; e < optimalNumEmptySquares; e++)
    {
        emptySquares.add(sampleItems.add(new SampleItem()));
    }
    
    // Sort sample item tiles with Fast Linear Assignment Sorting (FLAS)
    if (sampleItems.size() > 3)
    {
        mGridSorter.applySorting(sampleItems, optimalHeight, optimalWidth);
    }
    
    // Refill tile collection
    mSampleItemTiles.clear();
    
    for (SampleItem* sample : sampleItems)
    {
        addAndMakeVisible(mSampleItemTiles.add(new BlomeSampleItemTileView(sample, sampleLibrary, sampleItemPanel)));
    }
    
    int tileWidth = 85;
    mSampleTileGrid->items.addArray(mSampleItemTiles);
    setBounds(0,
              0,
              (optimalWidth * tileWidth) + (style->PANEL_MARGIN * 0.5),
              (optimalHeight * tileWidth) + (style->PANEL_MARGIN * 0.5));
    mSampleTileGrid->performLayout(Rectangle<int>(0,
                                                  0,
                                                  optimalWidth * tileWidth,
                                                  optimalHeight * tileWidth));
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
