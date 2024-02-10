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
    mSelectedSampleTileIndices.clear();
    
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
    emptySquares.clear();
    
    for (int e = 0; e < optimalNumEmptySquares; e++)
    {
        sampleItems.add(emptySquares.add(new SampleItem()));
    }
    
    // Sort sample item tiles with Fast Linear Assignment Sorting (FLAS)
    if (sampleItems.size() > 3)
    {
        mGridSorter->applySorting(optimalHeight, optimalWidth, false);
    }
    else
    {
        setupGrid();
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
    
    int minTileWidth = 75;
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

Point<int> BlomeSampleTileGridView::getTileCenter(BlomeSampleItemTileView *randomTile)
{
    Point<int> center = randomTile->getPosition();
    int halfTildWidth = randomTile->getWidth() / 2;
    center.addXY(halfTildWidth, halfTildWidth);
    
    return center;
}

Point<int> BlomeSampleTileGridView::selectRandomTile()
{
    int numTiles = mSampleItemTiles.size();
    int randomTileIndex = Random::getSystemRandom().nextInt(numTiles);
    deselectAll();
    BlomeSampleItemTileView* randomTile = mSampleItemTiles.getUnchecked(randomTileIndex);
    selectTile(randomTileIndex);
    
    return getTileCenter(randomTile);
}

void BlomeSampleTileGridView::loadSelectedTileIntoAudioPlayer()
{
    if (mSelectedSampleTileIndices.isEmpty())
    {
        return;
    }
    
    mSampleItemTiles.getUnchecked(mSelectedSampleTileIndices.getUnchecked(0))->loadIntoAudioPlayer();
}

void BlomeSampleTileGridView::selectAll()
{
    mSelectedSampleTileIndices.clear();
    
    for (int t = 0; t < mSampleItemTiles.size(); t++)
    {
        selectTile(t);
    }
    
    repaint();
}

void BlomeSampleTileGridView::deselectAll()
{
    for (int t : mSelectedSampleTileIndices)
    {
        mSampleItemTiles.getUnchecked(t)->setSelected(false);
    }
    
    mSelectedSampleTileIndices.clear();
    repaint();
}

Point<int> BlomeSampleTileGridView::selectLeft()
{
    int lastSelectedIndex = mSelectedSampleTileIndices.getLast();
    deselectAll();
    int newIndex = lastSelectedIndex % optimalWidth == 0 ? lastSelectedIndex : lastSelectedIndex - 1;
    
    return getTileCenter(selectTile(newIndex));
}

Point<int> BlomeSampleTileGridView::selectUp()
{
    int lastSelectedIndex = mSelectedSampleTileIndices.getLast();
    int newIndex = lastSelectedIndex - optimalWidth;
    deselectAll();
    
    return getTileCenter(selectTile(newIndex < 0 ? lastSelectedIndex : newIndex));
}

Point<int> BlomeSampleTileGridView::selectRight()
{
    int lastSelectedIndex = mSelectedSampleTileIndices.getLast();
    deselectAll();
    int newIndex = lastSelectedIndex % optimalWidth == optimalWidth - 1 ? lastSelectedIndex : lastSelectedIndex + 1;
    
    return getTileCenter(selectTile(newIndex));
}

Point<int> BlomeSampleTileGridView::selectDown()
{
    int lastSelectedIndex = mSelectedSampleTileIndices.getLast();
    int newIndex = lastSelectedIndex + optimalWidth;
    deselectAll();
    
    return getTileCenter(selectTile(newIndex > mSampleItemTiles.size() ? lastSelectedIndex : newIndex));
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
    
    for (SampleItem* sample : emptySquares)
    {
        sampleItems.removeAndReturn(sampleItems.indexOf(sample));
    }
    
    mSampleTileGrid->items.addArray(mSampleItemTiles);
    
    performGridLayout(currentZoomFactor);
    setVisible(true);
}

void BlomeSampleTileGridView::paint(Graphics& g)
{
    
}

void BlomeSampleTileGridView::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == &sampleLibrary && isShowing())
    {
        setVisible(false);
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

BlomeSampleItemTileView* BlomeSampleTileGridView::selectTile(int inTileIndex)
{
    mSelectedSampleTileIndices.add(inTileIndex);
    BlomeSampleItemTileView* tile = mSampleItemTiles.getUnchecked(inTileIndex);
    tile->setSelected(true);
    
    return tile;
    
}

void BlomeSampleTileGridView::deselectTile(int inTileIndex)
{
    mSelectedSampleTileIndices.removeAllInstancesOf(inTileIndex);
    mSampleItemTiles.getUnchecked(inTileIndex)->setSelected(false);
}

void BlomeSampleTileGridView::mouseUp(MouseEvent const & event)
{
    Point<int> mousePosition = event.getEventRelativeTo(this).getPosition();
    bool commandIsDown = event.mods.isCommandDown();
    bool shiftIsDown = event.mods.isShiftDown();
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
    if (!commandIsDown && !shiftIsDown)
    {
        deselectAll();
    }
    
    // Select or deselect clicked tile
    for (int t = 0; t < mSampleItemTiles.size(); t++)
    {
        BlomeSampleItemTileView* tile = mSampleItemTiles.getUnchecked(t);
        
        if (tile->getBoundsInParent().contains(mousePosition) && tile->getSampleItemFilePath() != "EMPTYTILE")
        {
            if (tile->getIsSelected())
            {
                deselectTile(t);
            }
            else
            {
                selectTile(t);
            }
        }
    }
    
    bool someSelected = !mSelectedSampleTileIndices.isEmpty();
    
    // Select all tiles between the last two selected tiles
    if (shiftIsDown && someSelected)
    {
        int numSelected = mSelectedSampleTileIndices.size();
        int startIndex = 0; // Default start from top left
        
        if (numSelected > 1)
        {
            startIndex = mSelectedSampleTileIndices.getReference(numSelected - 2);
        }
        
        int endIndex = mSelectedSampleTileIndices.getReference(numSelected - 1);
        
        int xStart = startIndex % optimalWidth;
        int yStart = startIndex / optimalWidth;
        int xEnd = endIndex % optimalWidth;
        int yEnd = endIndex / optimalWidth;
        
        if (xStart > xEnd)
        {
            int temp = xStart;
            xStart = xEnd;
            xEnd = temp;
        }
        
        if (yStart > yEnd)
        {
            int temp = yStart;
            yStart = yEnd;
            yEnd = temp;
        }
        
        for (int x = xStart; x <= xEnd; x++)
        {
            for (int y = yStart; y <= yEnd; y++)
            {
                int newSelectedIndex = y * optimalWidth + x;
                BlomeSampleItemTileView* tile = mSampleItemTiles.getUnchecked(newSelectedIndex);
                
                if (tile->getSampleItemFilePath() != "EMPTYTILE")
                {
                    selectTile(newSelectedIndex);
                }
            }
        }
    }
    
    // Show options pop up menu
    if (rightMouseIsDown && someSelected)
    {
        showPopupMenu();
    }
    
    repaint();
}

void BlomeSampleTileGridView::mouseDrag(MouseEvent const & mouseEvent)
{
    // If the drag was at least 50ms after the mouse was pressed
    if (mouseEvent.getLengthOfMousePress() > 100)
    {
        Point<int> mousePosition = mouseEvent.getEventRelativeTo(this).position.toInt();
        
        // Check if any of the selected tiles was dragged
        for (int s : mSelectedSampleTileIndices)
        {
            BlomeSampleItemTileView* tile = mSampleItemTiles.getUnchecked(s);
            Rectangle<int> tileBounds = tile->getBoundsInParent();
            
            if (tileBounds.contains(mousePosition))
            {
                StringArray selectedFilePaths;
                
                // Add all selected rows to external drag
                for (int r : mSelectedSampleTileIndices)
                {
                    selectedFilePaths.add(mSampleItemTiles.getUnchecked(r)->getSampleItemFilePath());
                }
                
                DragAndDropContainer* dragContainer = DragAndDropContainer::findParentDragContainerFor(this);
                dragContainer->performExternalDragDropOfFiles(selectedFilePaths, false, this);
                
                return;
            }
        }
    }
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
