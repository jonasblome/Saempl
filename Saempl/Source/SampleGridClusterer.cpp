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
ThreadPool(SystemStats::getNumCpus()),
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
    int numDimensions = (int) sampleItems.getFirst()->getFeatureVector().size();
    int gridSize = columns * rows;
    std::vector<std::vector<float>> grid;
    grid.resize(gridSize);
    for (int pos = 0; pos < grid.size(); pos++)
    {
        grid[pos] = std::vector<float>(numDimensions);
    }
    std::vector<float> weights;
    weights.resize(gridSize);
    numSwapPositions = jmin<int>(maxSwapPositions, columns * rows);
    
    // Calculate initial radius and number of reductions
    float rad = jmax<int>(columns, rows) * initialRadiusFactor;
    float utilRadius = rad;
    int numRadiusReductions = 0;
    
    while (utilRadius > endRadius)
    {
        utilRadius *= getRadiusDecay(utilRadius);
        numRadiusReductions++;
    }
    
    if (numRadiusReductions == 0)
    {
        return;
    }
    
    // Filter grid, define swap area and calculate optimal swaps
    int progressCounter = 0;
    while (rad >= endRadius)
    {
        if (threadShouldExit())
        {
            break;
        }
        
        setProgress(progressCounter++ * 1.0 / numRadiusReductions);
        
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
    
    while (swapAreaHeight * swapAreaWidth < numSwapPositions)
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
    
    int numSwapTries = jmax<int>(1, (sampleFactor * rows * columns / numSwapPositions));
    
    for (int n = 0; n < numSwapTries; n++)
    {
        addJob(new SampleSwapJob(sampleItems,
                                 numSwapPositions,
                                 applyWrap,
                                 swapAreaIndices,
                                 swapAreaWidth,
                                 swapAreaHeight,
                                 rows,
                                 columns,
                                 grid),
               true);
    }
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
