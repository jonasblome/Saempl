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
    sampleItemCollectionChanged = false;
    mGridSorter = std::make_unique<SampleItemGridSorter>(sampleLibrary.getSampleItems(FILTERED_SAMPLES));
    mGridSorter->addChangeListener(this);
    addMouseListener(this, true);
}

BlomeSampleTileGridView::~BlomeSampleTileGridView()
{
    sampleLibrary.removeChangeListener(this);
    mGridSorter->removeChangeListener(this);
    removeMouseListener(this);
}

void BlomeSampleTileGridView::sortGrid()
{
    if (!sampleItemCollectionChanged)
    {
        return;
    }
    
    // Retrieve sample items
    OwnedArray<SampleItem>& sampleItems = sampleLibrary.getSampleItems(FILTERED_SAMPLES);
    
    // Calculate optimal rectangle side lengths to minimise empty tiles,
    // while still trying to approximate a square grid
    int numTiles = sampleItems.size();
    int numItemsSqrt = std::ceil(sqrt(numTiles));
    int optimalNumEmptySquares = numItemsSqrt * numItemsSqrt - numTiles;
    optimalWidth = numItemsSqrt;
    optimalHeight = numItemsSqrt;
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
    
    // Add empty squares to sample items
    Array<SampleItem*> emptySquares;
    for (int e = 0; e < optimalNumEmptySquares; e++)
    {
        emptySquares.add(sampleItems.add(new SampleItem()));
    }
    
    // Sort sample item tiles with Fast Linear Assignment Sorting (FLAS)
    if (sampleItems.size() > 3)
    {
        mGridSorter->applySorting(optimalHeight, optimalWidth);
    }
    
    sampleItemCollectionChanged = false;
}

void BlomeSampleTileGridView::performGridLayout(float inZoomFactor)
{
    currentZoomFactor = inZoomFactor;
    
    if (mSampleItemTiles.isEmpty())
    {
        return;
    }
    
    int minTileWidth = 35;
    int maxTileWidth = 140;
    int tileWidth = minTileWidth + currentZoomFactor * (maxTileWidth - minTileWidth);
    
    setBounds(0,
              0,
              (optimalWidth * tileWidth) + (style->PANEL_MARGIN * 0.5),
              (optimalHeight * tileWidth) + (style->PANEL_MARGIN * 0.5));
    mSampleTileGrid->performLayout(Rectangle<int>(0,
                                                  0,
                                                  optimalWidth * tileWidth,
                                                  optimalHeight * tileWidth));
}

Point<int> BlomeSampleTileGridView::selectRandomTile()
{
    int numTiles = mSampleItemTiles.size();
    int randomTileIndex = Random::getSystemRandom().nextInt(numTiles);
    deselectAll();
    BlomeSampleItemTileView* randomTile = mSampleItemTiles.getUnchecked(randomTileIndex);
    mSelectedSampleTileIndices.add(randomTileIndex);
    randomTile->setSelected(true);
    
    return randomTile->getPosition();
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
    
    mSampleItemTiles.clear();
    
    OwnedArray<SampleItem>& sampleItems = sampleLibrary.getSampleItems(FILTERED_SAMPLES);
    for (SampleItem* sample : sampleItems)
    {
        addAndMakeVisible(mSampleItemTiles.add(new BlomeSampleItemTileView(sample, sampleLibrary, sampleItemPanel)));
    }
    
    mSampleTileGrid->items.addArray(mSampleItemTiles);
    
    performGridLayout(currentZoomFactor);
}

void BlomeSampleTileGridView::paint(Graphics& g)
{
    
}

void BlomeSampleTileGridView::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == &sampleLibrary && isShowing())
    {
        sampleItemCollectionChanged = true;
        sortGrid();
    }
    else if (source == &sampleLibrary && !isShowing())
    {
        sampleItemCollectionChanged = true;
    }
    else if (source == &*mGridSorter)
    {
        // Refill tile collection
        setupGrid();
    }
}

void BlomeSampleTileGridView::filesDropped(StringArray const & files, int x, int y)
{
    // Adding all the dropped files to the database
    for (int f = 0; f < files.size(); f++)
    {
        sampleLibrary.addAllToSampleItems(files[f]);
    }
}

bool BlomeSampleTileGridView::isInterestedInFileDrag(StringArray const & files)
{
    return true;
}

void BlomeSampleTileGridView::deselectAll()
{
    for (int t : mSelectedSampleTileIndices)
    {
        mSampleItemTiles.getUnchecked(t)->setSelected(false);
    }
    
    mSelectedSampleTileIndices.clear();
}

void BlomeSampleTileGridView::mouseUp(MouseEvent const & event)
{
    Point<int> mousePosition = event.getEventRelativeTo(this).getPosition();
    bool commandIsDown = event.mods.isCommandDown();
    bool rightMouseIsDown = event.mods.isRightButtonDown();
    
    // Try right click on selected tiles
    if (rightMouseIsDown)
    {
        for (int t = 0; t < mSelectedSampleTileIndices.size(); t++)
        {
            BlomeSampleItemTileView* tile = mSampleItemTiles.getUnchecked(mSelectedSampleTileIndices.getReference(t));
            
            if (tile->getBoundsInParent().contains(mousePosition))
            {
                showPopupMenu();
                return;
            }
        }
    }
    
    // Deselect all tiles
    if (!commandIsDown)
    {
        deselectAll();
    }
    
    // Select or deselect clicked tile
    for (int t = 0; t < mSampleItemTiles.size(); t++)
    {
        BlomeSampleItemTileView* tile = mSampleItemTiles.getUnchecked(t);
        
        if (tile->getBoundsInParent().contains(mousePosition) && tile->getSampleItemFilePath() != "")
        {
            if (tile->getIsSelected())
            {
                mSelectedSampleTileIndices.remove(t);
                tile->setSelected(false);
            }
            else
            {
                mSelectedSampleTileIndices.add(t);
                tile->setSelected(true);
            }
        }
    }
    
    // Show options pop up menu
    if (rightMouseIsDown && !mSelectedSampleTileIndices.isEmpty())
    {
        showPopupMenu();
    }
    
    repaint();
}

void BlomeSampleTileGridView::showPopupMenu()
{
    PopupMenu popupMenu;
    popupMenu.addItem("Move File(s) to Trash", [this] { deleteFiles(false); });
    popupMenu.addItem("Add Sample(s) to Favorites", [this] { addToPalette(); });
    popupMenu.addItem("Re-analyse Sample(s)", [this] { reanalyseSamples(); });
    popupMenu.addItem("Delete File(s) Permanently", [this] { deleteFiles(true); });
    popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
}

void BlomeSampleTileGridView::deleteFiles(bool deletePermanently = false)
{
    StringArray filePaths;
    
    for (int t : mSelectedSampleTileIndices)
    {
        filePaths.add(mSampleItemTiles.getUnchecked(t)->getSampleItemFilePath());
    }
    
    sampleLibrary.removeSampleItems(filePaths, deletePermanently);
}

void BlomeSampleTileGridView::addToPalette()
{
    StringArray filePaths;
    
    for (int t : mSelectedSampleTileIndices)
    {
        filePaths.add(mSampleItemTiles.getUnchecked(t)->getSampleItemFilePath());
    }
    
    sampleLibrary.addAllToPalette(filePaths);
}

void BlomeSampleTileGridView::reanalyseSamples()
{
    StringArray filePaths;
    
    for (int t : mSelectedSampleTileIndices)
    {
        filePaths.add(mSampleItemTiles.getUnchecked(t)->getSampleItemFilePath());
    }
    
    sampleLibrary.reanalyseSampleItems(filePaths);
}
