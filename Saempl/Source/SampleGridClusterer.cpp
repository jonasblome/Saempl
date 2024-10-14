/*
 ==============================================================================
 
 SampleGridClusterer.cpp
 Author:  Jonas Blome
 
 Translated and adapted code from the paper
 "Improved Evaluation and Generation of Grid Layouts using Distance Preservation Quality and Linear Assignment Sorting"
 by Kai Barthel, Nico Hezel, Klaus Jung, and Konstantin Schall.
 @HTW Berlin, Visual Computing Group, Germany
 https://visual-computing.com/
 The code was originally written by Nico Hezel, Konstantin Schall and Kai Barthel.
 
 ==============================================================================
 */

#include "SampleGridClusterer.h"

SampleGridClusterer::SampleGridClusterer(OwnedArray<SampleItem>& inSampleItems)
:
ThreadWithProgressWindow("Improving sample grid clustering quality", true, true, 10000, "Stop improving", nullptr),
sampleItems(inSampleItems)
{
    
}

SampleGridClusterer::~SampleGridClusterer()
{
    
}

void SampleGridClusterer::applyClustering(int inRows, int inColumns, bool doWrap)
{
    rows = inRows;
    columns = inColumns;
    applyWrap = doWrap;
    launchThread();
}

void SampleGridClusterer::setFeatureWeights(std::vector<float> inFeatureWeights)
{
    mFeatureWeights = inFeatureWeights;
    featureWeightsChanged = true;
}

void SampleGridClusterer::run()
{
    setProgress(0.0);
    
    // Assign input vectors to random grid positions
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(sampleItems.getRawDataPointer(),
                 sampleItems.getRawDataPointer() + sampleItems.size(),
                 std::default_random_engine(seed));
    
    // Copy weighted features to feature vectors
    if (featureWeightsChanged)
    {
        for (SampleItem* sample : sampleItems)
        {
            std::vector<float> featureVector = std::vector<float>(NUM_FEATURES + NUM_SPECTRAL_BANDS + NUM_CHROMA);
            
            featureVector[0] = sample->getLength() / 60 * mFeatureWeights[0];
            featureVector[1] = (sample->getLoudnessLUFS() + 300) / (3 + 300) * mFeatureWeights[1];
            featureVector[2] = (sample->getLoudnessLUFSEnd() - sample->getLoudnessLUFSStart()) * mFeatureWeights[2];
            featureVector[3] = sample->getZeroCrossingRate() * mFeatureWeights[3];
            featureVector[4] = (sample->getTempo() + LOWER_BPM_LIMIT) / (UPPER_BPM_LIMIT - LOWER_BPM_LIMIT) * mFeatureWeights[4];
            featureVector[5] = sample->getKey() * 1.0 / NUM_CHROMA * mFeatureWeights[5];
            featureVector[6] = sample->getSpectralCentroid() * mFeatureWeights[6];
            featureVector[7] = sample->getSpectralSpread() * mFeatureWeights[7];
            featureVector[8] = sample->getSpectralRolloff() * mFeatureWeights[8];
            featureVector[9] = sample->getSpectralFlux() * mFeatureWeights[9];
            featureVector[10] = sample->getChromaFlux() * mFeatureWeights[10];
            
            std::vector<float> spectralDistribution = sample->getSpectralDistribution();
            for (int sb = 0; sb < NUM_SPECTRAL_BANDS; sb++)
            {
                featureVector[NUM_FEATURES + sb] = spectralDistribution[sb] * mFeatureWeights[11];
            }
            
            std::vector<float> chromaDistribution = sample->getChromaDistribution();
            for (int c = 0; c < NUM_CHROMA; c++)
            {
                featureVector[NUM_FEATURES + NUM_SPECTRAL_BANDS + c] = chromaDistribution[c] * mFeatureWeights[12];
            }
            
            sample->setFeatureVector(featureVector);
        }
        
        featureWeightsChanged = false;
    }
    
    // Initialise vectors
    std::vector<std::vector<float>> grid;
    int gridSize = columns * rows;
    grid.resize(gridSize);
    int numDimensions = (int) sampleItems.getFirst()->getFeatureVector().size();
    for (int pos = 0; pos < grid.size(); pos++)
    {
        grid[pos] = std::vector<float>(numDimensions);
    }
    std::vector<float> weights;
    weights.resize(gridSize);
    int numSwapPositions = jmin<int>(maxSwapPositions, columns * rows);
    swapPositions.resize(numSwapPositions);
    swappedElements.resize(numSwapPositions);
    for (int s = 0; s < numSwapPositions; s++)
    {
        swappedElements[s] = sampleItems.getUnchecked(s);
    }
    mSwappedFeatureVectors.resize(numSwapPositions);
    mGridVectorsAtSwapPosition.resize(numSwapPositions);
    mDistanceMatrix.resize(numSwapPositions);
    mDistanceMatrixNormalised.resize(numSwapPositions);
    for (int pos = 0; pos < mDistanceMatrix.size(); pos++)
    {
        mDistanceMatrix[pos] = std::vector<float>(numDimensions);
        mDistanceMatrixNormalised[pos] = std::vector<int>(numDimensions);
    }
    
    // Calculate initial radius and number of reductions
    float rad = jmax<int>(columns, rows) * initialRadiusFactor;
    
    int numRadiusReductions = 0;
    float tempRad = rad;
    
    while (tempRad > endRadius)
    {
        tempRad *= getRadiusDecay(tempRad);
        numRadiusReductions++;
    }
    
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
            float weight = 1 / weights[pos];
            
            for (int d = 0; d < numDimensions; d++)
            {
                grid[pos][d] *= weight;
            }
        }
        
        // Find optimal random swaps
        checkRandomSwaps(radius, grid, rows, columns);
        
        // Reduce the filter radius
        rad *= getRadiusDecay(rad);
    }
}

void SampleGridClusterer::threadComplete(bool userPressedCancel)
{
    sendChangeMessage();
}

void SampleGridClusterer::copyFeatureVectorsToGrid(std::vector<std::vector<float>>& grid,
                                                   int numDimensions,
                                                   std::vector<float>& weights)
{
    for (int pos = 0; pos < grid.size(); pos++)
    {
        std::vector<float>& gridCell = grid[pos];
        SampleItem* sampleItem = sampleItems.getUnchecked(pos);
        
        if (sampleItem->getFilePath() != EMPTY_TILE_PATH)
        {
            for (int d = 0; d < numDimensions; d++)
            {
                gridCell[d] = sampleItem->getFeatureVector()[d] * weightTile;
            }
            
            weights[pos] = weightTile;
        }
        else // Hole
        {
            for (int d = 0; d < numDimensions; d++)
            {
                gridCell[d] *= weightHole;
            }
            
            weights[pos] = weightHole;
        }
    }
}

std::vector<std::vector<float>> SampleGridClusterer::filterHorizontallyWrap(std::vector<std::vector<float>>& inGrid,
                                                                            int rows,
                                                                            int columns,
                                                                            int numDimensions,
                                                                            int filterSize)
{
    if (columns == 1)
    {
        return inGrid;
    }
    
    std::vector<std::vector<float>> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    std::vector<std::vector<float>> extendedRow;
    extendedRow.resize(columns + 2 * borderExtension);
    
    // Filter the rows
    for (int row = 0; row < rows; row++)
    {
        int actualRowIndex = row * columns;
        
        // Copy the current row
        for (int col = 0; col < columns; col++)
        {
            extendedRow[col + borderExtension] = inGrid[actualRowIndex + col];
        }
        
        // Extend the row with wrapped values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedRow[borderExtension - 1 - e] = extendedRow[columns + borderExtension - e - 1];
            extendedRow[columns + borderExtension + e] = extendedRow[borderExtension + e];
        }
        
        std::vector<float> tmp = std::vector<float>(numDimensions);
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedRow[fp][d];
            }
        }
        
        output[actualRowIndex] = std::vector<float>(numDimensions);
        
        for (int d = 0; d < numDimensions; d++)
        {
            output[actualRowIndex][d] = tmp[d] / filterSize;
        }
        
        // Filter the rest of the row
        for (int col = 1; col < columns; col++)
        {
            output[actualRowIndex + col] = std::vector<float>(numDimensions);
            int left = col - 1;
            int right = left + filterSize;
            
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedRow[right][d] - extendedRow[left][d];
                output[actualRowIndex + col][d] = tmp[d] / filterSize;
            }
        }
    }
    
    return output;
}

std::vector<std::vector<float>> SampleGridClusterer::filterVerticallyWrap(std::vector<std::vector<float>>& inGrid,
                                                                          int rows,
                                                                          int columns,
                                                                          int numDimensions,
                                                                          int filterSize)
{
    if (rows == 1)
    {
        return inGrid;
    }
    
    std::vector<std::vector<float>> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    std::vector<std::vector<float>> extendedColumn;
    extendedColumn.resize(rows + 2 * borderExtension);
    
    // Filter the columns
    for (int col = 0; col < columns; col++)
    {
        // Copy the current column
        for (int row = 0; row < rows; row++)
        {
            extendedColumn[row + borderExtension] = inGrid[col + row * columns];
        }
        
        // Extend the column with wrapped values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedColumn[borderExtension - 1 - e] = extendedColumn[rows + borderExtension - e - 1];
            extendedColumn[rows + borderExtension + e] = extendedColumn[borderExtension + e];
        }
        
        std::vector<float> tmp = std::vector<float>(numDimensions);
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedColumn[fp][d];
            }
        }
        
        output[col] = std::vector<float>(numDimensions);
        
        for (int d = 0; d < numDimensions; d++)
        {
            output[col][d] = tmp[d] / filterSize;
        }
        
        // Filter the rest of the column
        for (int row = 1; row < rows; row++)
        {
            output[col + row * columns] = std::vector<float>(numDimensions);
            int left = row - 1;
            int right = left + filterSize;
            
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedColumn[right][d] - extendedColumn[left][d];
                output[col + row * columns][d] = tmp[d] / filterSize;
            }
        }
    }
    
    return output;
}

std::vector<float> SampleGridClusterer::filterHorizontallyWrap(std::vector<float>& inWeights,
                                                               int rows,
                                                               int columns,
                                                               int filterSize)
{
    if (columns == 1)
    {
        return inWeights;
    }
    
    std::vector<float> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    std::vector<float> extendedRow;
    extendedRow.resize(columns + 2 * borderExtension);
    
    // Filter the rows
    for (int row = 0; row < rows; row++)
    {
        int actualRowIndex = row * columns;
        
        // Copy the current row
        for (int col = 0; col < columns; col++)
        {
            extendedRow[col + borderExtension] = inWeights[actualRowIndex + col];
        }
        
        // Extend the row with wrapped values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedRow[borderExtension - 1 - e] = extendedRow[columns + borderExtension - e - 1];
            extendedRow[columns + borderExtension + e] = extendedRow[borderExtension + e];
        }
        
        float tmp = 0;
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            tmp += extendedRow[fp];
        }
        
        output[actualRowIndex] = tmp / filterSize;
        
        // Filter the rest of the row
        for (int col = 1; col < columns; col++)
        {
            int left = col - 1;
            int right = left + filterSize;
            tmp += extendedRow[right] - extendedRow[left];
            output[actualRowIndex + col] = tmp / filterSize;
        }
    }
    
    return output;
}

std::vector<float> SampleGridClusterer::filterVerticallyWrap(std::vector<float>& inWeights,
                                                             int rows,
                                                             int columns,
                                                             int filterSize)
{
    if (rows == 1)
    {
        return inWeights;
    }
    
    std::vector<float> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    std::vector<float> extendedColumn;
    extendedColumn.resize(rows + 2 * borderExtension);
    
    // Filter the columns
    for (int col = 0; col < columns; col++)
    {
        // Copy the current column
        for (int row = 0; row < rows; row++)
        {
            extendedColumn[row + borderExtension] = inWeights[col + row * columns];
        }
        
        // Extend the current column with wrapped values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedColumn[borderExtension - 1 - e] = extendedColumn[rows + borderExtension - e - 1];
            extendedColumn[rows + borderExtension + e] = extendedColumn[borderExtension + e];
        }
        
        float tmp = 0;
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            tmp += extendedColumn[fp];
        }
        
        output[col] = tmp / filterSize;
        
        // Filter the rest of the column
        for (int row = 1; row < rows; row++)
        {
            int left = row - 1;
            int right = left + filterSize;
            tmp += extendedColumn[right] - extendedColumn[left];
            output[col + row * columns] = tmp / filterSize;
        }
    }
    
    return output;
}

std::vector<std::vector<float>> SampleGridClusterer::filterHorizontallyMirror(std::vector<std::vector<float>>& inGrid,
                                                                              int rows,
                                                                              int columns,
                                                                              int numDimensions,
                                                                              int filterSize)
{
    if (columns == 1)
    {
        return inGrid;
    }
    
    std::vector<std::vector<float>> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    std::vector<std::vector<float>> extendedRow;
    extendedRow.resize(columns + 2 * borderExtension);
    
    // Filter the rows
    for (int row = 0; row < rows; row++)
    {
        int actualRowIndex = row * columns;
        
        // Copy the current row
        for (int col = 0; col < columns; col++)
        {
            extendedRow[col + borderExtension] = inGrid[actualRowIndex + col];
        }
        
        // Extend the row with mirrored values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedRow[borderExtension - 1 - e] = extendedRow[borderExtension + e + 1];
            extendedRow[columns + borderExtension + e] = extendedRow[columns + borderExtension - 2 - e];
        }
        
        std::vector<float> tmp = std::vector<float>(numDimensions);
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedRow[fp][d];
            }
        }
        
        output[actualRowIndex] = std::vector<float>(numDimensions);
        
        for (int d = 0; d < numDimensions; d++)
        {
            output[actualRowIndex][d] = tmp[d] / filterSize;
        }
        
        // Filter the rest of the row
        for (int col = 1; col < columns; col++)
        {
            output[actualRowIndex + col] = std::vector<float>(numDimensions);
            int left = col - 1;
            int right = left + filterSize;
            
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedRow[right][d] - extendedRow[left][d];
                output[actualRowIndex + col][d] = tmp[d] / filterSize;
            }
        }
    }
    
    return output;
}

std::vector<std::vector<float>> SampleGridClusterer::filterVerticallyMirror(std::vector<std::vector<float>>& inGrid,
                                                                            int rows,
                                                                            int columns,
                                                                            int numDimensions,
                                                                            int filterSize)
{
    if (rows == 1)
    {
        return inGrid;
    }
    
    std::vector<std::vector<float>> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    std::vector<std::vector<float>> extendedColumn;
    extendedColumn.resize(rows + 2 * borderExtension);
    
    // Filter the columns
    for (int col = 0; col < columns; col++)
    {
        // Copy the current column
        for (int row = 0; row < rows; row++)
        {
            extendedColumn[row + borderExtension] = inGrid[col + row * columns];
        }
        
        // Extend the column with mirrored values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedColumn[borderExtension - 1 - e] = extendedColumn[borderExtension + e + 1];
            extendedColumn[rows + borderExtension + e] = extendedColumn[borderExtension + rows - 2 - e];
        }
        
        std::vector<float> tmp = std::vector<float>(numDimensions);
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedColumn[fp][d];
            }
        }
        
        output[col] = std::vector<float>(numDimensions);
        
        for (int d = 0; d < numDimensions; d++)
        {
            output[col][d] = tmp[d] / filterSize;
        }
        
        // Filter the rest of the column
        for (int row = 1; row < rows; row++)
        {
            output[col + row * columns] = std::vector<float>(numDimensions);
            int left = row - 1;
            int right = left + filterSize;
            
            for (int d = 0; d < numDimensions; d++)
            {
                tmp[d] += extendedColumn[right][d] - extendedColumn[left][d];
                output[col + row * columns][d] = tmp[d] / filterSize;
            }
        }
    }
    
    return output;
}

std::vector<float> SampleGridClusterer::filterHorizontallyMirror(std::vector<float>& inWeights,
                                                                 int rows,
                                                                 int columns,
                                                                 int filterSize)
{
    if (columns == 1)
    {
        return inWeights;
    }
    
    std::vector<float> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    std::vector<float> extendedRow;
    extendedRow.resize(columns + 2 * borderExtension);
    
    // Filter the rows
    for (int row = 0; row < rows; row++)
    {
        int actualRowIndex = row * columns;
        
        // Copy the current row
        for (int col = 0; col < columns; col++)
        {
            extendedRow[col + borderExtension] = inWeights[actualRowIndex + col];
        }
        
        // Extend the row with mirrored values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedRow[borderExtension - 1 - e] = extendedRow[borderExtension + e + 1];
            extendedRow[columns + borderExtension + e] = extendedRow[columns + borderExtension - 2 - e];
        }
        
        float tmp = 0;
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            tmp += extendedRow[fp];
        }
        
        output[actualRowIndex] = tmp / filterSize;
        
        // Filter the rest of the row
        for (int col = 1; col < columns; col++)
        {
            int left = col - 1;
            int right = left + filterSize;
            tmp += extendedRow[right] - extendedRow[left];
            output[actualRowIndex + col] = tmp / filterSize;
        }
    }
    
    return output;
}

std::vector<float> SampleGridClusterer::filterVerticallyMirror(std::vector<float>& inWeights, int rows, int columns, int filterSize)
{
    if (rows == 1)
    {
        return inWeights;
    }
    
    std::vector<float> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    std::vector<float> extendedColumn;
    extendedColumn.resize(rows + 2 * borderExtension);
    
    // Filter the columns
    for (int col = 0; col < columns; col++)
    {
        // Copy the current column
        for (int row = 0; row < rows; row++)
        {
            extendedColumn[row + borderExtension] = inWeights[col + row * columns];
        }
        
        // Extend the current column with wrapped values
        for (int e = 0; e < borderExtension; e++)
        {
            extendedColumn[borderExtension - 1 - e] = extendedColumn[rows + borderExtension - e - 1];
            extendedColumn[rows + borderExtension + e] = extendedColumn[borderExtension + e];
        }
        
        float tmp = 0;
        
        // Filter the first element
        for (int fp = 0; fp < filterSize; fp++)
        {
            tmp += extendedColumn[fp];
        }
        
        output[col] = tmp / filterSize;
        
        // Filter the rest of the column
        for (int row = 1; row < rows; row++)
        {
            int left = row - 1;
            int right = left + filterSize;
            tmp += extendedColumn[right] - extendedColumn[left];
            output[col + row * columns] = tmp / filterSize;
        }
    }
    
    return output;
}

void SampleGridClusterer::checkRandomSwaps(int radius, std::vector<std::vector<float>>& grid, int rows, int columns)
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
    std::vector<int> swapAreaIndices;
    swapAreaIndices.resize(swapAreaWidth * swapAreaHeight);
    
    for (int i = 0, y = 0; y < swapAreaHeight; y++)
    {
        for (int x = 0; x < swapAreaWidth; x++)
        {
            swapAreaIndices[i++] = y * columns + x;
        }
    }
    
    // Shuffle swap indices
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(swapAreaIndices.begin(),
                 swapAreaIndices.begin() + swapAreaIndices.size(),
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

int SampleGridClusterer::findSwapPositionsWrap(std::vector<int>& swapAreaIndices,
                                               std::vector<int>& swapPositions,
                                               int swapAreaWidth,
                                               int swapAreaHeight,
                                               int rows,
                                               int columns)
{
    std::random_device random;
    std::mt19937 generator(random());
    
    std::uniform_int_distribution<> distribution1(0, (int) swapAreaIndices.size() - (int) swapPositions.size());
    int startIndex = (swapAreaIndices.size() - swapPositions.size() > 0) ? distribution1(generator) : 0;
    
    std::uniform_int_distribution<> distribution2(0, rows * columns);
    int randomPosition = distribution2(generator);
    int numSwapPositions = 0;
    for (int j = startIndex; j < swapAreaIndices.size() && numSwapPositions < swapPositions.size(); j++)
    {
        // Get wrapped position of random element to swap
        int d = randomPosition + swapAreaIndices[j];
        int x = d % columns;
        int y = (d / columns) % rows;
        int pos = y * columns + x;
        
        swapPositions[numSwapPositions++] = pos;
    }
    
    return (int) swapPositions.size();
}

int SampleGridClusterer::findSwapPositions(std::vector<int>& swapAreaIndices,
                                           std::vector<int>& swapPositions,
                                           int swapAreaWidth,
                                           int swapAreaHeight,
                                           int rows,
                                           int columns)
{
    std::random_device random;
    std::mt19937 generator(random());
    
    // Find random position in grid
    std::uniform_int_distribution<> distribution1(0, rows * columns);
    int randomPosition = distribution1(generator);
    int randomX = randomPosition % columns;
    int randomY = randomPosition / columns;
    
    // Limit swap area positions to grid bounds
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
    
    // Select random position in swap area
    std::uniform_int_distribution<> distribution2(0, (int) swapAreaIndices.size() - (int) swapPositions.size());
    int startIndex = (swapAreaIndices.size() - swapPositions.size() > 0) ? distribution2(generator) : 0;
    int numSwapPositions = 0;
    
    for (int j = startIndex; j < swapAreaIndices.size() && numSwapPositions < swapPositions.size(); j++)
    {
        // Get position of random element to swap
        int dx = swapAreaIndices[j] % columns;
        int dy = swapAreaIndices[j] / columns;
        
        int x = (xStart + dx) % columns;
        int y = (yStart + dy) % rows;
        int pos = y * columns + x;
        
        swapPositions[numSwapPositions++] = pos;
    }
    
    return (int) swapPositions.size();
}

void SampleGridClusterer::doSwaps(std::vector<int>& swapPositions,
                                  int numSwapPositions,
                                  std::vector<std::vector<float>>& grid)
{
    int numValid = 0;
    
    for (int s = 0; s < numSwapPositions; s++)
    {
        int swapPosition = swapPositions[s];
        SampleItem* swappedElement = sampleItems.getUnchecked(swapPosition);
        swappedElements[s] = swappedElement;
        
        // Handle holes
        if (swappedElement->getFilePath() != EMPTY_TILE_PATH)
        {
            mSwappedFeatureVectors[s] = swappedElement->getFeatureVector();
            numValid++;
        }
        else
        {
            // Hole
            mSwappedFeatureVectors[s] = grid[swapPosition];
        }
        
        mGridVectorsAtSwapPosition[s] = grid[swapPosition];
    }
    
    if (numValid > 0)
    {
        std::vector<std::vector<int>> distanceMatrix = calculateNormalisedDistanceMatrix(mSwappedFeatureVectors, mGridVectorsAtSwapPosition, numSwapPositions);
        std::vector<int> optimalPermutation = computeAssignment(distanceMatrix, numSwapPositions);
        
        for (int s = 0; s < numSwapPositions; s++)
        {
            sampleItems.set(swapPositions[optimalPermutation[s]],
                            swappedElements[s],
                            false);
        }
    }
}

std::vector<std::vector<int>> SampleGridClusterer::calculateNormalisedDistanceMatrix(std::vector<std::vector<float>>& inFeatureVectors,
                                                                                     std::vector<std::vector<float>>& inGridVectors,
                                                                                     int inSize)
{
    // Find maximum distance in the swapping area
    float maxDistance = 0;
    
    for (int i = 0; i < inSize; i++)
    {
        for (int j = 0; j < inSize; j++)
        {
            float currentDistance = calculateDistance(inFeatureVectors[i], inGridVectors[j]);
            mDistanceMatrix[i][j] = currentDistance;
            mDistanceMatrixNormalised[i][j] = 0;
            
            if (currentDistance > maxDistance)
            {
                maxDistance = currentDistance;
            }
        }
    }
    
    if (maxDistance == 0)
    {
        return mDistanceMatrixNormalised;
    }
    
    // Set normalised and quantised distances for the current swap area
    for (int i = 0; i < inSize; i++)
    {
        for (int j = 0; j < inSize; j++)
        {
            mDistanceMatrixNormalised[i][j] = (int) (QUANT
                                                     * mDistanceMatrix[i][j]
                                                     / maxDistance
                                                     + 0.5);
        }
    }
    
    return mDistanceMatrixNormalised;
}

float SampleGridClusterer::calculateDistance(std::vector<float> vector1, std::vector<float> vector2)
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

std::vector<int> SampleGridClusterer::computeAssignment(std::vector<std::vector<int>>& matrix, int numDimensions)
{
    int i, imin, i0, freerow;
    int j, j1, j2 = 0, endofpath = 0, last = 0, min = 0;
    
    std::vector<int> inRow;
    inRow.resize(numDimensions);
    std::vector<int> inCol;
    inCol.resize(numDimensions);
    std::vector<int> v;
    v.resize(numDimensions);
    std::vector<int> free;
    free.resize(numDimensions);
    std::vector<int> collist;
    collist.resize(numDimensions);
    std::vector<int> matches;
    matches.resize(numDimensions);
    std::vector<int> pred;
    pred.resize(numDimensions);
    std::vector<int> d;
    d.resize(numDimensions);
    
    // Skipping L53-54
    for (j = numDimensions - 1; j >= 0; j--)
    {
        min = matrix[0][j];
        imin = 0;
        
        for (i = 1; i < numDimensions; i++)
        {
            if (matrix[i][j] < min)
            {
                min = matrix[i][j];
                imin = i;
            }
        }
        
        v[j] = min;
        matches[imin]++;
        
        if (matches[imin] == 1)
        {
            inRow[imin] = j;
            inCol[j] = imin;
        }
        else
        {
            inCol[j] = -1;
        }
    }
    
    int numfree=0;
    
    for (i = 0; i < numDimensions; i++)
    {
        if (matches[i] == 0)
        {
            free[numfree] = i;
            numfree++;
        }
        else if (matches[i] == 1)
        {
            j1 = inRow[i];
            min = INT_MAX;
            
            for (j = 0; j < numDimensions; j++)
            {
                if (j != j1 && matrix[i][j] - v[j] < min)
                {
                    min = matrix[i][j] - v[j];
                }
            }
            
            v[j1] -= min;
        }
    }
    
    for (int loopcmt = 0; loopcmt < 2; loopcmt++)
    {
        int k = 0;
        int prvnumfree = numfree;
        numfree = 0;
        
        while (k < prvnumfree)
        {
            i = free[k];
            k++;
            int umin = matrix[i][0] - v[0];
            j1 = 0;
            int usubmin = INT_MAX;
            
            for (j = 1; j < numDimensions; j++)
            {
                int h = matrix[i][j] - v[j];
                
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
            
            i0 = inCol[j1];
            
            if (umin < usubmin)
            {
                v[j1] = v[j1] - (usubmin - umin);
            }
            else if (i0 >= 0)
            {
                j1 = j2;
                i0 = inCol[j2];
            }
            
            inRow[i] = j1;
            inCol[j1] = i;
            
            if (i0 >= 0)
            {
                if (umin < usubmin)
                {
                    k--;
                    free[k] = i0;
                }
                else
                {
                    free[numfree] = i0;
                    numfree++;
                }
            }
        }
    }
    
    for (int f = 0; f < numfree; f++)
    {
        freerow = free[f];
        
        for (j = 0; j < numDimensions; j++)
        {
            d[j] = matrix[freerow][j] - v[j];
            pred[j] = freerow;
            collist[j] = j;
        }
        
        int low = 0;
        int up = 0;
        bool unassignedfound = false;
        
        while (!unassignedfound)
        {
            if (up == low)
            {
                last = low - 1;
                min = d[collist[up]];
                up++;
                
                for (int k = up; k < numDimensions; k++)
                {
                    j = collist[k];
                    int h = d[j];
                    
                    if (h <= min)
                    {
                        if (h < min)
                        {
                            up = low;
                            min = h;
                        }
                        
                        collist[k] = collist[up];
                        collist[up] = j;
                        up++;
                    }
                }
                
                for (int k = low; k < up; k++)
                {
                    if (inCol[collist[k]] < 0)
                    {
                        endofpath = collist[k];
                        unassignedfound = true;
                        break;
                    }
                }
            }
            
            if (!unassignedfound)
            {
                j1 = collist[low];
                low++;
                i = inCol[j1];
                int h = matrix[i][j1] - v[j1] - min;
                
                for (int k = up; k < numDimensions; k++)
                {
                    j = collist[k];
                    int v2 = matrix[i][j] - v[j] - h;
                    
                    if (v2 < d[j])
                    {
                        pred[j] = i;
                        
                        if (v2 == min)
                        {
                            if (inCol[j] < 0)
                            {
                                endofpath = j;
                                unassignedfound = true;
                                break;
                            }
                            else
                            {
                                collist[k] = collist[up];
                                collist[up] = j;
                                up++;
                            }
                        }
                        
                        d[j] = v2;
                    }
                }
            }
        }
        
        for (int k = 0; k <= last; k++)
        {
            j1 = collist[k];
            v[j1] += d[j1] - min;
        }
        
        i = freerow + 1;
        while (i != freerow)
        {
            i = pred[endofpath];
            inCol[endofpath] = i;
            j1 = endofpath;
            endofpath = inRow[i];
            inRow[i] = j1;
        }
    }
    
    return inRow;
}

float SampleGridClusterer::getRadiusDecay(float inRadius)
{
    if (inRadius < 4)
    {
        return 0.95;
    }
    else if (inRadius > 100)
    {
        return 0.998;
    }
    
    float radiusDecay = 1.001 / (1 + 0.03555 * exp(-0.02348 * inRadius)); // Logistic Regression
    // float radiusDecay = 0.89901041667 + inRadius * 0.00098958333; // Linear
    // float radiusDecay = 0.975; // Default fix value
    
    return radiusDecay;
}
