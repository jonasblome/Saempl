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
    
    // Calculate optimal rectangle side lengths to minimise empty tiles,
    // while still trying to approximate a square grid
    int numTiles = mSampleItemTiles.size();
    int numItemsSqrt = std::ceil(sqrt(numTiles));
    int optimalNumEmptySquares = numItemsSqrt * numItemsSqrt - numTiles;
    int optimalWidth = numItemsSqrt;
    int optimalHeight = numItemsSqrt;
    int maximumLengthDeviation = 15;
    
    if (optimalNumEmptySquares != 0)
    {
        int currentMaximumLengthDeviation = jmin<int>(maximumLengthDeviation, std::ceil(numItemsSqrt * 0.35));
        
        for (int i = 1; i <= currentMaximumLengthDeviation; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                int numEmptySquares = (numItemsSqrt - i) * (numItemsSqrt + j) - numTiles;
                
                if (numEmptySquares < 0)
                {
                    // Grid is too small
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
    
    mSampleTileGrid->templateRows = Array<Track>();
    for (int i = 0; i < optimalHeight; i++)
    {
        mSampleTileGrid->templateRows.add(Track(1_fr));
    }
    mSampleTileGrid->templateColumns = Array<Track>();
    for (int i = 0; i < optimalWidth; i++)
    {
        mSampleTileGrid->templateColumns.add(Track(1_fr));
    }
    
    // Retrieve sample items
    OwnedArray<SampleItem> & sampleItems = sampleLibrary.getSampleItems(FILTERED_SAMPLES);
    
    // Sort sample item tiles with Fast Linear Assignment Sorting (FLAS)
    if (sampleItems.size() > 0)
    {
        mGridSorter.applySorting(sampleItems, optimalWidth, optimalHeight);
    }
    
    // Refill tile collection
    mSampleItemTiles.clear();
    
    for (SampleItem* sample : sampleItems)
    {
        addAndMakeVisible(mSampleItemTiles.add(new BlomeSampleItemTileView(sample, sampleLibrary, sampleItemPanel)));
    }
    
    mSampleTileGrid->items.addArray(mSampleItemTiles);
    setBounds(0,
              0,
              (optimalWidth * 100) + (style->PANEL_MARGIN * 0.5),
              (optimalHeight * 100) + (style->PANEL_MARGIN * 0.5));
    mSampleTileGrid->performLayout(Rectangle<int>(0,
                                                  0,
                                                  optimalWidth * 100,
                                                  optimalHeight * 100));
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
