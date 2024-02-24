/*
 ==============================================================================
 
 SampleItemGridClusterer.cpp
 Author:  Jonas Blome
 
 Translated and adapted code from the paper
 "Improved Evaluation and Generation of Grid Layouts using Distance Preservation Quality and Linear Assignment Sorting"
 by Kai Barthel, Nico Hezel, Klaus Jung, and Konstantin Schall.
 @HTW Berlin, Visual Computing Group, Germany
 https://visual-computing.com/
 The code was written by Nico Hezel, Konstantin Schall and Kai Barthel.
 
 ==============================================================================
 */

#include "SampleItemGridClusterer.h"

SampleItemGridClusterer::SampleItemGridClusterer(OwnedArray<SampleItem>& inSampleItems)
:
ThreadWithProgressWindow("Improving sample grid clustering quality", true, true, 10000, "Stop improving", nullptr),
sampleItems(inSampleItems)
{
    
}

SampleItemGridClusterer::~SampleItemGridClusterer()
{
    
}

void SampleItemGridClusterer::applyClustering(int inRows, int inColumns, bool doWrap)
{
    rows = inRows;
    columns = inColumns;
    applyWrap = doWrap;
    launchThread();
}

void SampleItemGridClusterer::run()
{
    setProgress(0.0);
    int gridSize = columns * rows;
    int numDimensions = (int) sampleItems.getFirst()->getFeatureVector().size();
    
    // Assign input vectors to random grid positions
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(sampleItems.getRawDataPointer(),
                 sampleItems.getRawDataPointer() + sampleItems.size(),
                 std::default_random_engine(seed));
    
    // Initialise arrays
    Array<std::vector<float>> grid;
    grid.resize(gridSize);
    for (int pos = 0; pos < grid.size(); pos++)
    {
        grid.set(pos, std::vector<float>(numDimensions));
    }
    Array<float> weights;
    weights.resize(gridSize);
    int numSwapPositions = jmin<int>(maxSwapPositions, columns * rows);
    swapPositions.resize(numSwapPositions);
    for (int s = 0; s < numSwapPositions; s++)
    {
        swappedElements.add(sampleItems.getUnchecked(s));
    }
    featureVectors.resize(numSwapPositions);
    gridFeatureVectors.resize(numSwapPositions);
    distanceMatrixNormalised.resize(numSwapPositions);
    distanceMatrix.resize(numSwapPositions);
    
    float rad = jmax<int>(columns, rows) * initialRadiusFactor;
    int numRadiusReductions = log(endRadius / rad) / log(radiusDecay);
    
    if (numRadiusReductions == 0)
    {
        return;
    }
    
    int i = 0;
    
    while (rad >= endRadius)
    {
        if (threadShouldExit())
        {
            break;
        }
        
        setProgress(i++ * 1.0 / numRadiusReductions);
        
        int radius = jmax<int>(1, std::round(rad));
        int radiusX = jmax<int>(1, jmin<int>(columns / 2, radius));
        int radiusY = jmax<int>(1, jmin<int>(rows / 2, radius));
        
        // Copy feature vectors to grid
        copyFeatureVectorsToGrid(grid, numDimensions, weights);
        
        // Apply filter
        int filterSizeX = 2 * radiusX + 1;
        int filterSizeY = 2 * radiusY + 1;
        
        if (applyWrap)
        {
            // Apply filter to grid
            grid = filterHorizontallyWrap(grid, rows, columns, numDimensions, filterSizeX);
            grid = filterVerticallyWrap(grid, rows, columns, numDimensions, filterSizeY);
            
            // Apply filter to weights
            weights = filterHorizontallyWrap(weights, rows, columns, filterSizeX);
            weights = filterVerticallyWrap(weights, rows, columns, filterSizeY);
        }
        else
        {
            // Apply filter to grid
            grid = filterHorizontallyMirror(grid, rows, columns, numDimensions, filterSizeX);
            grid = filterVerticallyMirror(grid, rows, columns, numDimensions, filterSizeY);
            
            // Apply filter to weights
            weights = filterHorizontallyMirror(weights, rows, columns, filterSizeX);
            weights = filterVerticallyMirror(weights, rows, columns, filterSizeY);
        }
        
        // Apply weights to grid vectors
        for (int pos = 0; pos < grid.size(); pos++)
        {
            float weight = 1 / weights.getReference(pos);
            
            for (int d = 0; d < numDimensions; d++)
            {
                grid.getReference(pos)[d] *= weight;
            }
        }
        
        // Find optimal random swaps
        checkRandomSwaps(radius, grid, rows, columns);
        
        // Reduce the filter radius
        rad *= radiusDecay;
    }
}

void SampleItemGridClusterer::threadComplete(bool userPressedCancel)
{
    sendChangeMessage();
}

void SampleItemGridClusterer::copyFeatureVectorsToGrid(Array<std::vector<float>>& grid, int numDimensions, Array<float>& weights)
{
    for (int pos = 0; pos < grid.size(); pos++)
    {
        std::vector<float>& gridCell = grid.getReference(pos);
        SampleItem* sampleItem = sampleItems.getUnchecked(pos);
        
        if (sampleItem->getFilePath() != EMPTY_TILE_PATH)
        {
            for (int d = 0; d < numDimensions; d++)
            {
                gridCell[d] = sampleItem->getFeatureVector()[d] * weightTile;
            }
            
            weights.set(pos, weightTile);
        }
        else // Hole
        {
            for (int d = 0; d < numDimensions; d++)
            {
                gridCell[d] *= weightHole;
            }
            
            weights.set(pos, weightHole);
        }
    }
}

Array<std::vector<float>> SampleItemGridClusterer::filterHorizontallyWrap(Array<std::vector<float>>& inGrid,
                                                                          int rows,
                                                                          int columns,
                                                                          int numDimensions,
                                                                          int filterSize)
{
    if (columns == 1)
    {
        return inGrid;
    }
    
    Array<std::vector<float>> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    Array<std::vector<float>> extendedRow;
    extendedRow.resize(columns + 2 * borderExtension);
    
    // Filter the rows
    for (int row = 0; row < rows; row++)
    {
        int actualRowIndex = row * columns;
        
        // Copy the current row
        for (int col = 0; col < columns; col++)
        {
            extendedRow.set(col + borderExtension, inGrid.getReference(actualRowIndex + col));
        }
        
        // Extend the row with wrapped values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedRow.set(borderExtension - 1 - e, extendedRow.getReference(columns + borderExtension - e - 1));
            extendedRow.set(columns + borderExtension + e, extendedRow.getReference(borderExtension + e));
        }
        
        std::vector<float> tmp = std::vector<float>(numDimensions);
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedRow.getReference(fp)[d];
            }
        }
        
        output.set(actualRowIndex, std::vector<float>(numDimensions));
        
        for (int d = 0; d < numDimensions; d++)
        {
            output.getReference(actualRowIndex)[d] = tmp[d] / filterSize;
        }
        
        // Filter the rest of the row
        for (int col = 1; col < columns; col++)
        {
            output.set(actualRowIndex + col, std::vector<float>(numDimensions));
            int left = col - 1;
            int right = left + filterSize;
            
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedRow.getReference(right)[d] - extendedRow.getReference(left)[d];
                output.getReference(actualRowIndex + col)[d] = tmp[d] / filterSize;
            }
        }
    }
    
    return output;
}

Array<std::vector<float>> SampleItemGridClusterer::filterVerticallyWrap(Array<std::vector<float>>& inGrid,
                                                                        int rows,
                                                                        int columns,
                                                                        int numDimensions,
                                                                        int filterSize)
{
    if (rows == 1)
    {
        return inGrid;
    }
    
    Array<std::vector<float>> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    Array<std::vector<float>> extendedColumn;
    extendedColumn.resize(rows + 2 * borderExtension);
    
    // Filter the columns
    for (int col = 0; col < columns; col++)
    {
        // Copy the current column
        for (int row = 0; row < rows; row++)
        {
            extendedColumn.set(row + borderExtension, inGrid.getReference(col + row * columns));
        }
        
        // Extend the column with wrapped values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedColumn.set(borderExtension - 1 - e, extendedColumn.getReference(rows + borderExtension - e - 1));
            extendedColumn.set(rows + borderExtension + e, extendedColumn.getReference(borderExtension + e));
        }
        
        std::vector<float> tmp = std::vector<float>(numDimensions);
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedColumn.getReference(fp)[d];
            }
        }
        
        output.set(col, std::vector<float>(numDimensions));
        
        for (int d = 0; d < numDimensions; d++)
        {
            output.getReference(col)[d] = tmp[d] / filterSize;
        }
        
        // Filter the rest of the column
        for (int row = 1; row < rows; row++)
        {
            output.set(col + row * columns, std::vector<float>(numDimensions));
            int left = row - 1;
            int right = left + filterSize;
            
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedColumn.getReference(right)[d] - extendedColumn.getReference(left)[d];
                output.getReference(col + row * columns)[d] = tmp[d] / filterSize;
            }
        }
    }
    
    return output;
}

Array<float> SampleItemGridClusterer::filterHorizontallyWrap(Array<float>& inWeights,
                                                             int rows,
                                                             int columns,
                                                             int filterSize)
{
    if (columns == 1)
    {
        return inWeights;
    }
    
    Array<float> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    Array<float> extendedRow;
    extendedRow.resize(columns + 2 * borderExtension);
    
    // Filter the rows
    for (int row = 0; row < rows; row++)
    {
        int actualRowIndex = row * columns;
        
        // Copy the current row
        for (int col = 0; col < columns; col++)
        {
            extendedRow.set(col + borderExtension, inWeights.getReference(actualRowIndex + col));
        }
        
        // Extend the row with wrapped values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedRow.set(borderExtension - 1 - e, extendedRow.getReference(columns + borderExtension - e - 1));
            extendedRow.set(columns + borderExtension + e, extendedRow.getReference(borderExtension + e));
        }
        
        float tmp = 0;
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            tmp += extendedRow.getReference(fp);
        }
        
        output.set(actualRowIndex, tmp / filterSize);
        
        // Filter the rest of the row
        for (int col = 1; col < columns; col++)
        {
            int left = col - 1;
            int right = left + filterSize;
            tmp += extendedRow.getReference(right) - extendedRow.getReference(left);
            output.set(actualRowIndex + col, tmp / filterSize);
        }
    }
    
    return output;
}

Array<float> SampleItemGridClusterer::filterVerticallyWrap(Array<float>& inWeights,
                                                           int rows,
                                                           int columns,
                                                           int filterSize)
{
    if (rows == 1)
    {
        return inWeights;
    }
    
    Array<float> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    Array<float> extendedColumn;
    extendedColumn.resize(rows + 2 * borderExtension);
    
    // Filter the columns
    for (int col = 0; col < columns; col++)
    {
        // Copy the current column
        for (int row = 0; row < rows; row++)
        {
            extendedColumn.set(row + borderExtension, inWeights.getReference(col + row * columns));
        }
        
        // Extend the current column with wrapped values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedColumn.set(borderExtension - 1 - e, extendedColumn.getReference(rows + borderExtension - e - 1));
            extendedColumn.set(rows + borderExtension + e, extendedColumn.getReference(borderExtension + e));
        }
        
        float tmp = 0;
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            tmp += extendedColumn.getReference(fp);
        }
        
        output.set(col, tmp / filterSize);
        
        // Filter the rest of the column
        for (int row = 1; row < rows; row++)
        {
            int left = row - 1;
            int right = left + filterSize;
            tmp += extendedColumn.getReference(right) - extendedColumn.getReference(left);
            output.set(col + row * columns, tmp / filterSize);
        }
    }
    
    return output;
}

Array<std::vector<float>> SampleItemGridClusterer::filterHorizontallyMirror(Array<std::vector<float>>& inGrid,
                                                                            int rows,
                                                                            int columns,
                                                                            int numDimensions,
                                                                            int filterSize)
{
    if (columns == 1)
    {
        return inGrid;
    }
    
    Array<std::vector<float>> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    Array<std::vector<float>> extendedRow;
    extendedRow.resize(columns + 2 * borderExtension);
    
    // Filter the rows
    for (int row = 0; row < rows; row++)
    {
        int actualRowIndex = row * columns;
        
        // Copy the current row
        for (int col = 0; col < columns; col++)
        {
            extendedRow.set(col + borderExtension, inGrid.getReference(actualRowIndex + col));
        }
        
        // Extend the row with mirrored values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedRow.set(borderExtension - 1 - e, extendedRow.getReference(borderExtension + e + 1));
            extendedRow.set(columns + borderExtension + e, extendedRow.getReference(columns + borderExtension - 2 - e));
        }
        
        std::vector<float> tmp = std::vector<float>(numDimensions);
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedRow.getReference(fp)[d];
            }
        }
        
        output.set(actualRowIndex, std::vector<float>(numDimensions));
        
        for (int d = 0; d < numDimensions; d++)
        {
            output.getReference(actualRowIndex)[d] = tmp[d] / filterSize;
        }
        
        // Filter the rest of the row
        for (int col = 1; col < columns; col++)
        {
            output.set(actualRowIndex + col, std::vector<float>(numDimensions));
            int left = col - 1;
            int right = left + filterSize;
            
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedRow.getReference(right)[d] - extendedRow.getReference(left)[d];
                output.getReference(actualRowIndex + col)[d] = tmp[d] / filterSize;
            }
        }
    }
    
    return output;
}

Array<std::vector<float>> SampleItemGridClusterer::filterVerticallyMirror(Array<std::vector<float>>& inGrid,
                                                                          int rows,
                                                                          int columns,
                                                                          int numDimensions,
                                                                          int filterSize)
{
    if (rows == 1)
    {
        return inGrid;
    }
    
    Array<std::vector<float>> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    Array<std::vector<float>> extendedColumn;
    extendedColumn.resize(rows + 2 * borderExtension);
    
    // Filter the columns
    for (int col = 0; col < columns; col++)
    {
        // Copy the current column
        for (int row = 0; row < rows; row++)
        {
            extendedColumn.set(row + borderExtension, inGrid.getReference(col + row * columns));
        }
        
        // Extend the column with mirrored values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedColumn.set(borderExtension - 1 - e, extendedColumn.getReference(borderExtension + e + 1));
            extendedColumn.set(rows + borderExtension + e, extendedColumn.getReference(borderExtension + rows - 2 - e));
        }
        
        std::vector<float> tmp = std::vector<float>(numDimensions);
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedColumn.getReference(fp)[d];
            }
        }
        
        output.set(col, std::vector<float>(numDimensions));
        
        for (int d = 0; d < numDimensions; d++)
        {
            output.getReference(col)[d] = tmp[d] / filterSize;
        }
        
        // Filter the rest of the column
        for (int row = 1; row < rows; row++)
        {
            output.set(col + row * columns, std::vector<float>(numDimensions));
            int left = row - 1;
            int right = left + filterSize;
            
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedColumn.getReference(right)[d] - extendedColumn.getReference(left)[d];
                output.getReference(col + row * columns)[d] = tmp[d] / filterSize;
            }
        }
    }
    
    return output;
}

Array<float> SampleItemGridClusterer::filterHorizontallyMirror(Array<float>& inWeights, int rows, int columns, int filterSize)
{
    if (columns == 1)
    {
        return inWeights;
    }
    
    Array<float> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    Array<float> extendedRow;
    extendedRow.resize(columns + 2 * borderExtension);
    
    // Filter the rows
    for (int row = 0; row < rows; row++)
    {
        int actualRowIndex = row * columns;
        
        // Copy the current row
        for (int col = 0; col < columns; col++)
        {
            extendedRow.set(col + borderExtension, inWeights.getReference(actualRowIndex + col));
        }
        
        // Extend the row with mirrored values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedRow.set(borderExtension - 1 - e, extendedRow.getReference(borderExtension + e + 1));
            extendedRow.set(columns + borderExtension + e, extendedRow.getReference(columns + borderExtension - 2 - e));
        }
        
        float tmp = 0;
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            tmp += extendedRow.getReference(fp);
        }
        
        output.set(actualRowIndex, tmp / filterSize);
        
        // Filter the rest of the row
        for (int col = 1; col < columns; col++)
        {
            int left = col - 1;
            int right = left + filterSize;
            tmp += extendedRow.getReference(right) - extendedRow.getReference(left);
            output.set(actualRowIndex + col, tmp / filterSize);
        }
    }
    
    return output;
}

Array<float> SampleItemGridClusterer::filterVerticallyMirror(Array<float>& inWeights, int rows, int columns, int filterSize)
{
    if (rows == 1)
    {
        return inWeights;
    }
    
    Array<float> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    Array<float> extendedColumn;
    extendedColumn.resize(rows + 2 * borderExtension);
    
    // Filter the columns
    for (int col = 0; col < columns; col++)
    {
        // Copy the current column
        for (int row = 0; row < rows; row++)
        {
            extendedColumn.set(row + borderExtension, inWeights.getReference(col + row * columns));
        }
        
        // Extend the current column with wrapped values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedColumn.set(borderExtension - 1 - e, extendedColumn.getReference(rows + borderExtension - e - 1));
            extendedColumn.set(rows + borderExtension + e, extendedColumn.getReference(borderExtension + e));
        }
        
        float tmp = 0;
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            tmp += extendedColumn.getReference(fp);
        }
        
        output.set(col, tmp / filterSize);
        
        // Filter the rest of the column
        for (int row = 1; row < rows; row++)
        {
            int left = row - 1;
            int right = left + filterSize;
            tmp += extendedColumn.getReference(right) - extendedColumn.getReference(left);
            output.set(col + row * columns, tmp / filterSize);
        }
    }
    
    return output;
}

void SampleItemGridClusterer::checkRandomSwaps(int radius, Array<std::vector<float>>& grid, int rows, int columns)
{
    // Set swap size
    int swapAreaWidth = jmin<int>(2 * radius + 1, columns);
    int swapAreaHeight = jmin<int>(2 * radius + 1, rows);
    int k = 0;
    
    while (swapAreaHeight * swapAreaWidth < swapPositions.size())
    {
        // Alternate between width and height with the size increase
        if ((k++ & 0x1) == 0)
        {
            swapAreaWidth = jmin<int>(swapAreaWidth + 1, columns);
        }
        else
        {
            swapAreaHeight = jmin<int>(swapAreaHeight + 1, rows);
        }
    }
    
    // Get all positions of the actual swap region
    Array<int> swapAreaIndices;
    swapAreaIndices.resize(swapAreaWidth * swapAreaHeight);
    
    for (int i = 0, y = 0; y < swapAreaHeight; y++)
    {
        for (int x = 0; x < swapAreaWidth; x++)
        {
            swapAreaIndices.set(i++, y * columns + x);
        }
    }
    
    // Shuffle swap indices
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(swapAreaIndices.getRawDataPointer(),
                 swapAreaIndices.getRawDataPointer() + swapAreaIndices.size(),
                 std::default_random_engine(seed));
    
    int numSwapTries = jmax<int>(1, (sampleFactor * rows * columns / swapPositions.size()));
    
    if (applyWrap)
    {
        for (int n = 0; n < numSwapTries; n++)
        {
            int numSwapPositions = findSwapPositionsWrap(swapAreaIndices,
                                                         swapPositions,
                                                         swapAreaWidth,
                                                         swapAreaHeight,
                                                         rows,
                                                         columns);
            doSwaps(swapPositions, numSwapPositions, grid);
        }
    }
    else
    {
        for (int n = 0; n < numSwapTries; n++)
        {
            int numSwapPositions = findSwapPositions(swapAreaIndices,
                                                     swapPositions,
                                                     swapAreaWidth,
                                                     swapAreaHeight,
                                                     rows,
                                                     columns);
            doSwaps(swapPositions, numSwapPositions, grid);
        }
    }
}

int SampleItemGridClusterer::findSwapPositionsWrap(Array<int>& swapAreaIndices, Array<int>& swapPositions, int swapAreaWidth, int swapAreaHeight, int rows, int columns)
{
    std::random_device random;
    std::mt19937 generator(random());
    
    std::uniform_int_distribution<> distribution1(0, swapAreaIndices.size() - swapPositions.size());
    int startIndex = (swapAreaIndices.size() - swapPositions.size() > 0) ? distribution1(generator) : 0;
    
    std::uniform_int_distribution<> distribution2(0, rows * columns);
    int randomPosition = distribution2(generator);
    int numSwapPositions = 0;
    for (int j = startIndex; j < swapAreaIndices.size() && numSwapPositions < swapPositions.size(); j++)
    {
        // Get wrapped position of random element to swap
        int d = randomPosition + swapAreaIndices.getReference(j);
        int x = d % columns;
        int y = (d / columns) % rows;
        int pos = y * columns + x;
        
        swapPositions.set(numSwapPositions++, pos);
    }
    
    return swapPositions.size();
}

int SampleItemGridClusterer::findSwapPositions(Array<int>& swapAreaIndices, Array<int>& swapPositions, int swapAreaWidth, int swapAreaHeight, int rows, int columns)
{
    std::random_device random;
    std::mt19937 generator(random());
    
    std::uniform_int_distribution<> distribution1(0, rows * columns);
    int randomPosition = distribution1(generator);
    int randomX = randomPosition % columns;
    int randomY = randomPosition / columns;
    
    int xStart = jmax<int>(0, randomX - swapAreaWidth / 2);
    int yStart = jmax<int>(0, randomY - swapAreaHeight / 2);
    
    if (xStart + swapAreaWidth > columns)
    {
        xStart = columns - swapAreaWidth;
    }
    
    if (yStart + swapAreaHeight > rows)
    {
        yStart = rows - swapAreaHeight;
    }
    
    std::uniform_int_distribution<> distribution2(0, swapAreaIndices.size() - swapPositions.size());
    int startIndex = (swapAreaIndices.size() - swapPositions.size() > 0) ? distribution2(generator) : 0;
    int numSwapPositions = 0;
    for (int j = startIndex; j < swapAreaIndices.size() && numSwapPositions < swapPositions.size(); j++)
    {
        // Get position of random element to swap
        int dx = swapAreaIndices.getReference(j) % columns;
        int dy = swapAreaIndices.getReference(j) / columns;
        
        int x = (xStart + dx) % columns;
        int y = (yStart + dy) % rows;
        int pos = y * columns + x;
        
        swapPositions.set(numSwapPositions++, pos);
    }
    
    return swapPositions.size();
}

void SampleItemGridClusterer::doSwaps(Array<int>& swapPositions,
                                      int numSwapPositions,
                                      Array<std::vector<float>>& grid)
{
    int numValid = 0;
    
    for (int s = 0; s < numSwapPositions; s++)
    {
        int swapPosition = swapPositions.getReference(s);
        SampleItem* swappedElement = sampleItems.getUnchecked(swapPosition);
        swappedElements.set(s, swappedElement);
        
        // Handle holes
        if (swappedElement->getFilePath() != EMPTY_TILE_PATH)
        {
            featureVectors.set(s, swappedElement->getFeatureVector());
            numValid++;
        }
        else
        {
            // Hole
            featureVectors.set(s, grid.getReference(swapPosition));
        }
        
        gridFeatureVectors.set(s, grid.getReference(swapPosition));
    }
    
    if (numValid > 0)
    {
        Array<Array<int>> distanceMatrix = calculateNormalisedDistanceMatrix(featureVectors, gridFeatureVectors, numSwapPositions);
        Array<int> optimalPermutation = computeAssignment(distanceMatrix, numSwapPositions);
        
        for (int s = 0; s < numSwapPositions; s++)
        {
            sampleItems.set(swapPositions.getReference(optimalPermutation.getReference(s)),
                            swappedElements.getUnchecked(s),
                            false);
        }
    }
}

Array<Array<int>> SampleItemGridClusterer::calculateNormalisedDistanceMatrix(Array<std::vector<float>>& inFeatureVectors, Array<std::vector<float>>& inGridVectors, int inSize)
{
    // Find maximum distance in the swapping area
    float maxDistance = 0;
    
    for (int i = 0; i < inSize; i++)
    {
        for (int j = 0; j < inSize; j++)
        {
            float currentDistance = calculateDistance(inFeatureVectors.getReference(i), inGridVectors.getReference(j));
            distanceMatrix.getReference(i).set(j, currentDistance);
            distanceMatrixNormalised.getReference(i).set(j, 0);
            
            if (currentDistance > maxDistance)
            {
                maxDistance = currentDistance;
            }
        }
    }
    
    if (maxDistance == 0)
    {
        return distanceMatrixNormalised;
    }
    
    // Set normalised and quantised distances for the current swap area
    for (int i = 0; i < inSize; i++)
    {
        for (int j = 0; j < inSize; j++)
        {
            distanceMatrixNormalised.getReference(i).set(j, (int) (QUANT
                                                                   * distanceMatrix.getReference(i).getReference(j)
                                                                   / maxDistance
                                                                   + 0.5));
        }
    }
    
    return distanceMatrixNormalised;
}

float SampleItemGridClusterer::calculateDistance(std::vector<float> vector1, std::vector<float> vector2)
{
    float distance = 0.0;
    
    int numDimensions = jmin<int>((int) vector1.size(), (int) vector2.size());
    for (int d = 0; d < numDimensions; d++)
    {
        float dist = vector1[d] - vector2[d];
        
        // Wrap it around for the dimension that represents the key
        if (d == 6)
        {
            dist = abs(dist);
            
            if (dist > 0.5)
            {
                dist = 1 - dist;
            }
        }
        
        distance += dist * dist;
    }
    
    return sqrt(distance);
}

Array<int> SampleItemGridClusterer::computeAssignment(Array<Array<int>>& matrix, int numDimensions)
{
    int i, imin, i0, freerow;
    int j, j1, j2 = 0, endofpath = 0, last = 0, min = 0;
    
    Array<int> inRow;
    inRow.resize(numDimensions);
    Array<int> inCol;
    inCol.resize(numDimensions);
    Array<int> v;
    v.resize(numDimensions);
    Array<int> free;
    free.resize(numDimensions);
    Array<int> collist;
    collist.resize(numDimensions);
    Array<int> matches;
    matches.resize(numDimensions);
    Array<int> pred;
    pred.resize(numDimensions);
    Array<int> d;
    d.resize(numDimensions);
    
    // Skipping L53-54
    for (j = numDimensions - 1; j >= 0; j--)
    {
        min = matrix.getReference(0).getReference(j);
        imin = 0;
        
        for (i = 1; i < numDimensions; i++)
        {
            if (matrix.getReference(i).getReference(j) < min)
            {
                min = matrix.getReference(i).getReference(j);
                imin = i;
            }
        }
        
        v.set(j, min);
        matches.getReference(imin)++;
        
        if (matches.getReference(imin) == 1)
        {
            inRow.set(imin, j);
            inCol.set(j, imin);
        }
        else
        {
            inCol.set(j, -1);
        }
    }
    
    int numfree=0;
    
    for (i = 0; i < numDimensions; i++)
    {
        if (matches.getReference(i) == 0)
        {
            free.set(numfree, i);
            numfree++;
        }
        else if (matches.getReference(i) == 1)
        {
            j1 = inRow.getReference(i);
            min = INT_MAX;
            
            for (j = 0; j < numDimensions; j++)
            {
                if (j != j1 && matrix.getReference(i).getReference(j) - v.getReference(j) < min)
                {
                    min = matrix.getReference(i).getReference(j) - v.getReference(j);
                }
            }
            
            v.getReference(j1) -= min;
        }
    }
    
    for (int loopcmt = 0; loopcmt < 2; loopcmt++)
    {
        int k = 0;
        int prvnumfree = numfree;
        numfree = 0;
        
        while (k < prvnumfree)
        {
            i = free.getReference(k);
            k++;
            int umin = matrix.getReference(i).getReference(0) - v.getReference(0);
            j1 = 0;
            int usubmin = INT_MAX;
            
            for (j = 1; j < numDimensions; j++)
            {
                int h = matrix.getReference(i).getReference(j) - v.getReference(j);
                
                if (h < usubmin)
                {
                    if (h >= umin)
                    {
                        usubmin = h;
                        j2 = j;
                    }
                    else
                    {
                        usubmin = umin;
                        umin = h;
                        j2 = j1;
                        j1 = j;
                    }
                }
            }
            
            i0 = inCol.getReference(j1);
            
            if (umin < usubmin)
            {
                v.set(j1, v.getReference(j1) - (usubmin - umin));
            }
            else if (i0 >= 0)
            {
                j1 = j2;
                i0 = inCol.getReference(j2);
            }
            
            inRow.set(i, j1);
            inCol.set(j1, i);
            
            if (i0 >= 0)
            {
                if (umin < usubmin)
                {
                    k--;
                    free.set(k, i0);
                }
                else
                {
                    free.set(numfree, i0);
                    numfree++;
                }
            }
        }
    }
    
    for (int f = 0; f < numfree; f++)
    {
        freerow = free.getReference(f);
        
        for (j = 0; j < numDimensions; j++)
        {
            d.set(j, matrix.getReference(freerow).getReference(j) - v.getReference(j));
            pred.set(j, freerow);
            collist.set(j, j);
        }
        
        int low = 0;
        int up = 0;
        bool unassignedfound = false;
        
        while (!unassignedfound)
        {
            if (up == low)
            {
                last = low - 1;
                min = d.getReference(collist.getReference(up));
                up++;
                
                for (int k = up; k < numDimensions; k++)
                {
                    j = collist.getReference(k);
                    int h = d.getReference(j);
                    
                    if (h <= min)
                    {
                        if (h < min)
                        {
                            up = low;
                            min = h;
                        }
                        
                        collist.set(k, collist.getReference(up));
                        collist.set(up, j);
                        up++;
                    }
                }
                
                for (int k = low; k < up; k++)
                {
                    if (inCol.getReference(collist.getReference(k)) < 0)
                    {
                        endofpath = collist.getReference(k);
                        unassignedfound = true;
                        break;
                    }
                }
            }
            
            if (!unassignedfound)
            {
                j1 = collist.getReference(low);
                low++;
                i = inCol.getReference(j1);
                int h = matrix.getReference(i).getReference(j1) - v.getReference(j1) - min;
                
                for (int k = up; k < numDimensions; k++)
                {
                    j = collist.getReference(k);
                    int v2 = matrix.getReference(i).getReference(j) - v.getReference(j) - h;
                    
                    if (v2 < d.getReference(j))
                    {
                        pred.set(j, i);
                        
                        if (v2 == min)
                        {
                            if (inCol.getReference(j) < 0)
                            {
                                endofpath = j;
                                unassignedfound = true;
                                break;
                            }
                            else
                            {
                                collist.set(k, collist.getReference(up));
                                collist.set(up, j);
                                up++;
                            }
                        }
                        
                        d.set(j, v2);
                    }
                }
            }
        }
        
        for (int k = 0; k <= last; k++)
        {
            j1 = collist.getReference(k);
            v.getReference(j1) += d.getReference(j1) - min;
        }
        
        i = freerow + 1;
        while (i != freerow)
        {
            i = pred.getReference(endofpath);
            inCol.getReference(endofpath) = i;
            j1 = endofpath;
            endofpath = inRow.getReference(i);
            inRow.set(i, j1);
        }
    }
    
    return inRow;
}
