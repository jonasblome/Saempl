/*
 ==============================================================================
 
 BlomeSampleGridView.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeSampleGridView.h"

BlomeSampleGridView::BlomeSampleGridView(SaemplAudioProcessor& inProcessor,
                                         SampleItemPanel& inSampleItemPanel,
                                         AudioPlayer& inAudioPlayer)
:
currentProcessor(inProcessor),
sampleLibrary(inProcessor.getSampleLibrary()),
audioPlayer(inAudioPlayer),
sampleItemPanel(inSampleItemPanel)
{
    sampleLibrary.addChangeListener(this);
    sampleItemCollectionChanged = false;
    addMouseListener(this, true);
    
    // Add grid clusterer
    mGridClusterer = std::make_unique<SampleGridClusterer>(sampleLibrary.getSampleItems(FILTERED_SAMPLES));
    mGridClusterer->addChangeListener(this);
}

BlomeSampleGridView::~BlomeSampleGridView()
{
    sampleLibrary.removeChangeListener(this);
    mGridClusterer->removeChangeListener(this);
    removeMouseListener(this);
}

void BlomeSampleGridView::clusterGrid()
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
    
    // Cluster sample item tiles with Fast Linear Assignment Sorting (FLAS)
    if (sampleItems.size() > 3)
    {
        if (currentProcessor.getFeatureWeightsChanged())
        {
            mGridClusterer->setFeatureWeights(currentProcessor.getFeatureWeights());
            currentProcessor.setFeatureWeightsChanged(false);
        }
        
        mGridClusterer->applyClustering(optimalHeight, optimalWidth, false);
    }
    else
    {
        setupGrid();
    }
    
    sampleItemCollectionChanged = false;
}

void BlomeSampleGridView::performGridLayout()
{
    if (mSampleTiles.isEmpty())
    {
        return;
    }
    
    int tileWidth = minTileWidth + currentZoomFactor * (maxTileWidth - minTileWidth);
    float minMargin = style->PANEL_MARGIN * 0.5f;
    float maxMargin = minMargin * getTileMinMaxRelation();
    float margin = minMargin + currentZoomFactor * (maxMargin - minMargin);
    mSampleGrid->setGap(Grid::Px(margin));
    setBounds(0,
              0,
              optimalWidth * tileWidth,
              optimalHeight * tileWidth);
    mSampleGrid->performLayout(Rectangle<int>(0,
                                                  0,
                                                  optimalWidth * tileWidth,
                                                  optimalHeight * tileWidth));
}

void BlomeSampleGridView::setZoomFactor(float inZoomFactor)
{
    currentZoomFactor = inZoomFactor;
}

Point<int> BlomeSampleGridView::getTileCentre(BlomeSampleTileView *randomTile)
{
    Point<int> centre = randomTile->getPosition();
    int halfTildWidth = randomTile->getWidth() / 2;
    centre.addXY(halfTildWidth, halfTildWidth);
    
    return centre;
}

Point<int> BlomeSampleGridView::selectRandomTile()
{
    int numTiles = mSampleTiles.size();
    
    if (numTiles == 0)
    {
        return Point<int>();
    }
    
    int randomTileIndex = Random::getSystemRandom().nextInt(numTiles);
    deselectAll();
    BlomeSampleTileView* randomTile = mSampleTiles.getUnchecked(randomTileIndex);
    selectTile(randomTileIndex);
    
    return getTileCentre(randomTile);
}

void BlomeSampleGridView::loadSelectedTileIntoAudioPlayer()
{
    if (mSelectedSampleTileIndices.isEmpty())
    {
        return;
    }
    
    mSampleTiles.getUnchecked(mSelectedSampleTileIndices.getUnchecked(0))->loadIntoAudioPlayer();
}

void BlomeSampleGridView::selectAll()
{
    mSelectedSampleTileIndices.clear();
    
    for (int t = 0; t < mSampleTiles.size(); t++)
    {
        selectTile(t);
    }
    
    repaint();
}

void BlomeSampleGridView::deselectAll()
{
    for (int t : mSelectedSampleTileIndices)
    {
        mSampleTiles.getUnchecked(t)->setSelected(false);
    }
    
    mSelectedSampleTileIndices.clear();
    repaint();
}

Point<int> BlomeSampleGridView::selectLeft()
{
    int lastSelectedIndex = mSelectedSampleTileIndices.getLast();
    deselectAll();
    int newIndex = lastSelectedIndex % optimalWidth == 0 ? lastSelectedIndex : lastSelectedIndex - 1;
    
    return getTileCentre(selectTile(newIndex));
}

Point<int> BlomeSampleGridView::selectUp()
{
    int lastSelectedIndex = mSelectedSampleTileIndices.getLast();
    int newIndex = lastSelectedIndex - optimalWidth;
    deselectAll();
    
    return getTileCentre(selectTile(newIndex < 0 ? lastSelectedIndex : newIndex));
}

Point<int> BlomeSampleGridView::selectRight()
{
    int lastSelectedIndex = mSelectedSampleTileIndices.getLast();
    deselectAll();
    int newIndex = lastSelectedIndex % optimalWidth == optimalWidth - 1 ? lastSelectedIndex : lastSelectedIndex + 1;
    
    return getTileCentre(selectTile(newIndex));
}

Point<int> BlomeSampleGridView::selectDown()
{
    int lastSelectedIndex = mSelectedSampleTileIndices.getLast();
    int newIndex = lastSelectedIndex + optimalWidth;
    deselectAll();
    
    return getTileCentre(selectTile(newIndex >= mSampleTiles.size() ? lastSelectedIndex : newIndex));
}

void BlomeSampleGridView::setReadyForClustering()
{
    sampleItemCollectionChanged = true;
}

void BlomeSampleGridView::setupGrid()
{
    // Setup tile grid
    mSampleTiles.clear();
    OwnedArray<SampleItem>& sampleItems = sampleLibrary.getSampleItems(FILTERED_SAMPLES);
    mSampleGrid = std::make_unique<Grid>();
    setVisible(true);
    
    if (sampleItems.size() == 0)
    {
        return;
    }
    
    mSampleGrid->autoFlow = Grid::AutoFlow::row;
    using Track = Grid::TrackInfo;
    mSampleGrid->autoRows = Track(1_fr);
    mSampleGrid->autoColumns = Track(1_fr);
    
    // Set up grid rows
    mSampleGrid->templateRows = Array<Track>();
    for (int i = 0; i < optimalHeight; i++)
    {
        mSampleGrid->templateRows.add(Track(1_fr));
    }
    
    // Set up grid columns
    mSampleGrid->templateColumns = Array<Track>();
    for (int i = 0; i < optimalWidth; i++)
    {
        mSampleGrid->templateColumns.add(Track(1_fr));
    }
    
    // Setup tiles
    for (SampleItem* sample : sampleItems)
    {
        addAndMakeVisible(mSampleTiles.add(new BlomeSampleTileView(sample, sampleLibrary, sampleItemPanel, audioPlayer)));
    }
    
    // Remove empty sample items from collection
    // but keep them for the tile views
    for (SampleItem* sample : emptySquares)
    {
        sampleItems.removeAndReturn(sampleItems.indexOf(sample));
    }
    
    mSampleGrid->items.addArray(mSampleTiles);
    performGridLayout();
}

void BlomeSampleGridView::paint(Graphics& g)
{
    
}

void BlomeSampleGridView::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == &sampleLibrary && isShowing())
    {
        setVisible(false);
        setReadyForClustering();
        clusterGrid();
    }
    else if (source == &sampleLibrary && !isShowing())
    {
        setVisible(false);
        setReadyForClustering();
    }
    else if (source == &*mGridClusterer)
    {
        // Refill tile collection
        setupGrid();
    }
}

void BlomeSampleGridView::filesDropped(StringArray const & files, int x, int y)
{
    // Adding all the dropped files to the database
    sampleLibrary.addAllToSampleItems(files);
}

bool BlomeSampleGridView::isInterestedInFileDrag(StringArray const & files)
{
    return true;
}

BlomeSampleTileView* BlomeSampleGridView::selectTile(int inTileIndex)
{
    mSelectedSampleTileIndices.add(inTileIndex);
    BlomeSampleTileView* tile = mSampleTiles.getUnchecked(inTileIndex);
    tile->setSelected(true);
    
    return tile;
    
}

void BlomeSampleGridView::deselectTile(int inTileIndex)
{
    mSelectedSampleTileIndices.removeAllInstancesOf(inTileIndex);
    mSampleTiles.getUnchecked(inTileIndex)->setSelected(false);
}

void BlomeSampleGridView::mouseUp(MouseEvent const & event)
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
            BlomeSampleTileView* tile = mSampleTiles.getUnchecked(mSelectedSampleTileIndices.getReference(t));
            
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
    for (int t = 0; t < mSampleTiles.size(); t++)
    {
        BlomeSampleTileView* tile = mSampleTiles.getUnchecked(t);
        
        if (tile->getBoundsInParent().contains(mousePosition) && tile->getSampleItemFilePath() != EMPTY_TILE_PATH)
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
            mSelectedSampleTileIndices.remove(numSelected - 2);
            numSelected--;
        }
        
        int endIndex = mSelectedSampleTileIndices.getReference(numSelected - 1);
        mSelectedSampleTileIndices.remove(numSelected - 1);
        
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
                BlomeSampleTileView* tile = mSampleTiles.getUnchecked(newSelectedIndex);
                
                if (tile->getSampleItemFilePath() != EMPTY_TILE_PATH)
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

void BlomeSampleGridView::mouseDrag(MouseEvent const & mouseEvent)
{
    // If the drag was at least 50ms after the mouse was pressed
    if (mouseEvent.getLengthOfMousePress() > 100)
    {
        Point<int> mousePosition = mouseEvent.getEventRelativeTo(this).position.toInt();
        
        // Check if any of the selected tiles was dragged
        for (int s : mSelectedSampleTileIndices)
        {
            BlomeSampleTileView* tile = mSampleTiles.getUnchecked(s);
            Rectangle<int> tileBounds = tile->getBoundsInParent();
            
            if (tileBounds.contains(mousePosition))
            {
                StringArray selectedFilePaths;
                
                // Add all selected rows to external drag
                for (int r : mSelectedSampleTileIndices)
                {
                    selectedFilePaths.add(mSampleTiles.getUnchecked(r)->getSampleItemFilePath());
                }
                
                DragAndDropContainer* dragContainer = DragAndDropContainer::findParentDragContainerFor(this);
                dragContainer->performExternalDragDropOfFiles(selectedFilePaths, false, this);
                
                return;
            }
        }
    }
}

void BlomeSampleGridView::showPopupMenu()
{
    PopupMenu popupMenu;
    popupMenu.addItem("Move File(s) to Trash", [this] { deleteFiles(false); });
    popupMenu.addItem("Add Sample(s) to Favourites", [this] { addToFavourites(); });
    popupMenu.addItem("(Re-)analyse Sample(s)", [this] { reanalyseSamples(); });
    popupMenu.addItem("Delete File(s) Permanently", [this] { deleteFiles(true); });
    popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
}

void BlomeSampleGridView::deleteFiles(bool deletePermanently = false)
{
    StringArray filePaths;
    
    for (int t : mSelectedSampleTileIndices)
    {
        filePaths.add(mSampleTiles.getUnchecked(t)->getSampleItemFilePath());
    }
    
    sampleLibrary.removeSampleItems(filePaths, deletePermanently);
}

void BlomeSampleGridView::addToFavourites()
{
    StringArray filePaths;
    
    for (int t : mSelectedSampleTileIndices)
    {
        filePaths.add(mSampleTiles.getUnchecked(t)->getSampleItemFilePath());
    }
    
    sampleLibrary.addAllToFavourites(filePaths);
}

void BlomeSampleGridView::reanalyseSamples()
{
    StringArray filePaths;
    
    for (int t : mSelectedSampleTileIndices)
    {
        filePaths.add(mSampleTiles.getUnchecked(t)->getSampleItemFilePath());
    }
    
    sampleLibrary.reanalyseSampleItems(filePaths);
}

float BlomeSampleGridView::getTileMinMaxRelation()
{
    return maxTileWidth * 1.0 / minTileWidth;
}

void BlomeSampleGridView::playSelectedSample()
{
    if (mSelectedSampleTileIndices.isEmpty())
    {
        return;
    }
    
    mSampleTiles.getUnchecked(mSelectedSampleTileIndices.getReference(0))->startPlayback();
}
