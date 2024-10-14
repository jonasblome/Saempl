/*
 ==============================================================================
 
 SampleAnalysisJob.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleSwapJob.h"

SampleSwapJob::SampleSwapJob(OwnedArray<SampleItem>& inSampleItems,
                             int inNumSwapPositions,
                             bool inApplyWrap,
                             std::vector<int> inSwapAreaIndices,
                             int inSwapAreaWidth,
                             int inSwapAreaHeight,
                             int inRows,
                             int inColumns,
                             std::vector<std::vector<float>> & inGrid)
:
ThreadPoolJob("SampleSwapJob"),
numSwapPositions(inNumSwapPositions),
applyWrap(inApplyWrap),
swapAreaWidth(inSwapAreaWidth),
swapAreaHeight(inSwapAreaHeight),
rows(inRows),
columns(inColumns),
sampleItems(inSampleItems),
grid(inGrid)
{
    
}

SampleSwapJob::~SampleSwapJob()
{
    
}

ThreadPoolJob::JobStatus SampleSwapJob::runJob()
{
    int numDimensions = (int) sampleItems.getFirst()->getFeatureVector().size();
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
    
    int numActualSwapPositions = 0;
    
    if (applyWrap)
    {
        numActualSwapPositions = findSwapPositionsWrap(swapAreaIndices,
                                                     swapPositions,
                                                     swapAreaWidth,
                                                     swapAreaHeight,
                                                     rows,
                                                     columns);
    }
    else
    {
        numActualSwapPositions = findSwapPositions(swapAreaIndices,
                                                 swapPositions,
                                                 swapAreaWidth,
                                                 swapAreaHeight,
                                                 rows,
                                                 columns);
    }
    
    doSwaps(swapPositions, numActualSwapPositions, grid);
    
    return jobHasFinished;
}

int SampleSwapJob::findSwapPositionsWrap(std::vector<int>& swapAreaIndices,
                                         std::vector<int>& swapPositions,
                                         int swapAreaWidth,
                                         int swapAreaHeight,
                                         int rows,
                                         int columns)
{
    std::random_device random;
    std::mt19937 generator(random());
    
    std::uniform_int_distribution<> distribution1(0, (int) swapAreaIndices.size() - (int) numSwapPositions);
    int startIndex = (swapAreaIndices.size() - numSwapPositions > 0) ? distribution1(generator) : 0;
    
    std::uniform_int_distribution<> distribution2(0, rows * columns);
    int randomPosition = distribution2(generator);
    int numActualSwapPositions = 0;
    for (int j = startIndex; j < swapAreaIndices.size() && numActualSwapPositions < numSwapPositions; j++)
    {
        // Get wrapped position of random element to swap
        int d = randomPosition + swapAreaIndices[j];
        int x = d % columns;
        int y = (d / columns) % rows;
        int pos = y * columns + x;
        
        swapPositions[numActualSwapPositions++] = pos;
    }
    
    return numActualSwapPositions;
}

int SampleSwapJob::findSwapPositions(std::vector<int>& swapAreaIndices,
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
    std::uniform_int_distribution<> distribution2(0, (int) swapAreaIndices.size() - numSwapPositions);
    int startIndex = (swapAreaIndices.size() - numSwapPositions > 0) ? distribution2(generator) : 0;
    int numActualSwapPositions = 0;
    
    for (int sp = startIndex; sp < swapAreaIndices.size() && numActualSwapPositions < numSwapPositions; sp++)
    {
        // Get position of random element to swap
        int dx = swapAreaIndices[sp] % columns;
        int dy = swapAreaIndices[sp] / columns;
        
        int x = (xStart + dx) % columns;
        int y = (yStart + dy) % rows;
        int pos = y * columns + x;
        
        swapPositions[numActualSwapPositions++] = pos;
    }
    
    return numSwapPositions;
}

void SampleSwapJob::doSwaps(std::vector<int>& swapPositions,
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

std::vector<std::vector<int>> SampleSwapJob::calculateNormalisedDistanceMatrix(std::vector<std::vector<float>>& inFeatureVectors,
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

float SampleSwapJob::calculateDistance(std::vector<float> vector1, std::vector<float> vector2)
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

std::vector<int> SampleSwapJob::computeAssignment(std::vector<std::vector<int>>& matrix, int numDimensions)
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
