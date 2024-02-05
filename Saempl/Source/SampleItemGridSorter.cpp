/*
  ==============================================================================

    SampleItemGridSorter.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleItemGridSorter.h"

SampleItemGridSorter::SampleItemGridSorter()
{
    
}

SampleItemGridSorter::~SampleItemGridSorter()
{
    
}

void SampleItemGridSorter::applySorting(OwnedArray<SampleItem>& inSampleItems,
                                        int gridWidth,
                                        int gridHeight)
{
    int gridSize = gridWidth * gridHeight;
    int numDimensions = (int) inSampleItems.getFirst()->getFeatureVector().size();
    
    Array<std::vector<float>> grid;
    grid.resize(gridSize);
    Array<float> weights;
    weights.resize(gridSize);
    
    swapPositions.resize(jmin<int>(maxSwapPositions, gridWidth * gridHeight));
    for (int s = 0; s < swapPositions.size(); s++)
    {
        swappedElements.add(inSampleItems.getUnchecked(s));
    }
    featureVectors.resize(swapPositions.size());
    gridFeatureVectors.resize(swapPositions.size());
    distLut.resize(swapPositions.size());
    distLutF.resize(swapPositions.size());
    
    float rad = jmax<int>(gridWidth, gridHeight) * initialRadiusFactor;
    
    // Assign input vectors to random grid positions
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(inSampleItems.getRawDataPointer(),
                 inSampleItems.getRawDataPointer() + inSampleItems.size(),
                 std::default_random_engine(seed));
    
    while (rad >= endRadius)
    {
        int radius = jmax<int>(1, std::round(rad));
        int radiusX = jmax<int>(1, jmin<int>(gridHeight / 2, radius));
        int radiusY = jmax<int>(1, jmin<int>(gridWidth / 2, radius));
        
        // Copy feature vectors to grid
        copyFeatureVectorsToGrid(grid, inSampleItems, numDimensions, weights);
        
        // Apply filter
        int filterSizeX = 2 * radiusX + 1;
        int filterSizeY = 2 * radiusY + 1;
        
        // Apply filter to grid
        grid = filterHorizontally(grid, gridHeight, gridWidth, numDimensions, filterSizeX);
        grid = filterVertically(grid, gridHeight, gridWidth, numDimensions, filterSizeY);
        
        // Apply filter to weights
        weights = filterHorizontally(weights, gridHeight, gridWidth, filterSizeX);
        weights = filterVertically(weights, gridHeight, gridWidth, filterSizeY);
    
        // Apply weights to grid vectors
        for (int i = 0; i < grid.size(); i++)
        {
            float weight = 1 / weights.getReference(i);
            
            for (int d = 0; d < numDimensions; d++)
            {
                grid.getReference(i)[d] *= weight;
            }
        }
        
        checkRandomSwaps(radius, inSampleItems, grid, gridHeight, gridWidth);
        
        // Reduce the filter radius
        rad *= radiusDecay;
    }
}

void SampleItemGridSorter::copyFeatureVectorsToGrid(Array<std::vector<float>>& grid, OwnedArray<SampleItem>& inSampleItems, int numDimensions, Array<float> weights)
{
    for (int pos = 0; pos < grid.size(); pos++)
    {
        grid.set(pos, std::vector<float>(numDimensions));
        std::vector<float>& gridCell = grid.getReference(pos);
        
        if (pos < inSampleItems.size())
        {
            std::vector<float> const & featureVector = inSampleItems.getUnchecked(pos)->getFeatureVector();
            
            for (int d = 0; d < numDimensions; d++)
            {
                gridCell[d] = featureVector[d] * weightTile;
            }
            
            weights.set(pos, weightTile);
        }
        else
        {
            for (int d = 0; d < numDimensions; d++)
            {
                gridCell[d] *= weightHole;
            }
            
            weights.set(pos, weightTile);
        }
    }
}

Array<std::vector<float>> SampleItemGridSorter::filterHorizontally(Array<std::vector<float>>& input, int rows, int columns, int dims, int filterSize)
{
    if (columns == 1)
    {
        return input;
    }
    
    Array<std::vector<float>> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    Array<std::vector<float>> extendedRow;
    extendedRow.resize(columns + 2 * borderExtension);

    // Filter the rows
    for (int y = 0; y < rows; y++)
    {
        int actRow = y * columns;

        for (int i = 0; i < columns; i++)
        {
            extendedRow.set(i + borderExtension, input.getReference(actRow + i)); // Copy one row
        }

        // Wrapped extension
        for (int i = 0; i < borderExtension; i++)
        {
            extendedRow.set(borderExtension - 1 - i, extendedRow.getReference(columns + borderExtension - i - 1));
            extendedRow.set(columns + borderExtension + i, extendedRow.getReference(borderExtension + i));
        }

        std::vector<float> tmp = std::vector<float>(dims);
        
        // First element
        for (int i = 0; i < filterSize; i++)
        {
            for (int d = 0; d < dims; d++)
            {
                tmp[d] += extendedRow.getReference(i)[d];
            }
        }
        
        output.set(actRow, std::vector<float>(dims));
        
        for (int d = 0; d < dims; d++)
        {
            output.getReference(actRow)[d] = tmp[d] / filterSize;
        }
        
        // Rest of the row
        for (int i = 1; i < columns; i++)
        {
            output.set(actRow + i, std::vector<float>(dims));
            int left = i - 1;
            int right = left + filterSize;

            for (int d = 0; d < dims; d++)
            {
                tmp[d] += extendedRow.getReference(right)[d] - extendedRow.getReference(left)[d];
                output.getReference(actRow + i)[d] = tmp[d] / filterSize;
            }
        }
    }
    
    return output;
}

Array<std::vector<float>> SampleItemGridSorter::filterVertically(Array<std::vector<float>>& input, int rows, int columns, int dims, int filterSize)
{
    if (rows == 1)
    {
        return input;
    }
    
    Array<std::vector<float>> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    Array<std::vector<float>> extendedColumn;
    extendedColumn.resize(rows + 2 * borderExtension);
    
    // Filter the columns
    for (int x = 0; x < columns; x++)
    {
        for (int i = 0; i < rows; i++)
        {
            extendedColumn.set(i + borderExtension, input.getReference(x + i * columns)); // Copy one column
        }
    
        // Wrapped extension
        for (int i = 0; i < borderExtension; i++)
        {
            extendedColumn.set(borderExtension - 1 - i, extendedColumn.getReference(rows + borderExtension - i - 1));
            extendedColumn.set(rows + borderExtension + i, extendedColumn.getReference(borderExtension + i));
        }

        std::vector<float> tmp = std::vector<float>(dims);
        
        for (int i = 0; i < filterSize; i++) // First element
        {
            for (int d = 0; d < dims; d++)
            {
                tmp[d] += extendedColumn.getReference(i)[d];
            }
        }
        
        output.set(x, std::vector<float>(dims));
        
        for (int d = 0; d < dims; d++)
        {
            output.getReference(x)[d] = tmp[d] / filterSize;
        }

        for (int i = 1; i < rows; i++) // Rest of the column
        {
            output.set(x + i * columns, std::vector<float>(dims));
            int left = i-1;
            int right = left + filterSize;
            
            for (int d = 0; d < dims; d++)
            {
                tmp[d] += extendedColumn.getReference(right)[d] - extendedColumn.getReference(left)[d];
                output.getReference(x + i * columns)[d] = tmp[d] / filterSize;
            }
        }
    }
    
    return output;
}

Array<float> SampleItemGridSorter::filterHorizontally(Array<float>& input, int rows, int columns, int filterSize)
{
    if (columns == 1)
    {
        return input;
    }
    
    Array<float> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    Array<float> extendedRow;
    extendedRow.resize(columns + 2 * borderExtension);

    // Filter the rows
    for (int y = 0; y < rows; y++)
    {
        int actRow = y * columns;

        for (int i = 0; i < columns; i++)
        {
            extendedRow.set(i + borderExtension, input.getReference(actRow + i)); // Copy one row
        }

        // Wrapped extension
        for (int i = 0; i < borderExtension; i++)
        {
            extendedRow.set(borderExtension - 1 - i, extendedRow.getReference(columns + borderExtension - i - 1));
            extendedRow.set(columns + borderExtension + i, extendedRow.getReference(borderExtension + i));
        }

        float tmp = 0;
        
        for (int i = 0; i < filterSize; i++) // First element
        {
            tmp += extendedRow.getReference(i);
        }

        output.set(actRow, tmp / filterSize);

        for (int i = 1; i < columns; i++) // Rest of the row
        {
            int left = i-1;
            int right = left + filterSize;
            tmp += extendedRow.getReference(right) - extendedRow.getReference(left);
            output.set(actRow + i, tmp / filterSize);
        }
    }
    
    return output;
}

Array<float> SampleItemGridSorter::filterVertically(Array<float>& input, int rows, int columns, int filterSize)
{
    if (rows == 1)
    {
        return input;
    }
    
    Array<float> output;
    output.resize(rows * columns);
    int borderExtension = filterSize / 2;
    Array<float> extendedColumn;
    extendedColumn.resize(rows + 2 * borderExtension);
    
    // Filter the columns
    for (int x = 0; x < columns; x++)
    {
        for (int i = 0; i < rows; i++)
        {
            extendedColumn.set(i + borderExtension, input.getReference(x + i * columns)); // Copy one column
        }
    
        // Wrapped extension
        for (int i = 0; i < borderExtension; i++)
        {
            extendedColumn.set(borderExtension - 1 - i, extendedColumn.getReference(rows + borderExtension - i - 1));
            extendedColumn.set(rows + borderExtension + i, extendedColumn.getReference(borderExtension + i));
        }

        float tmp = 0;
        
        for (int i = 0; i < filterSize; i++) // First element
        {
            tmp += extendedColumn.getReference(i);
        }

        output.set(x, tmp / filterSize);

        for (int i = 1; i < rows; i++) // Rest of the column
        {
            int left = i-1;
            int right = left + filterSize;
            tmp += extendedColumn.getReference(right) - extendedColumn.getReference(left);
            output.set(x + i * columns, tmp / filterSize);
        }
    }
    
    return output;
}

void SampleItemGridSorter::checkRandomSwaps(int radius, OwnedArray<SampleItem>& inSampleItems, Array<std::vector<float>> grid, int rows, int columns)
{
    // Set swap size
    int swapAreaWidth = jmin<int>(2 * radius + 1, columns);
    int swapAreaHeight = jmin<int>(2 * radius + 1, rows);
    int k = 0;
    
    while (swapAreaHeight * swapAreaWidth < swapPositions.size())
    {
        // Alternate the size increase
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
    Array<int> swapIndices;
    swapIndices.resize(swapAreaWidth * swapAreaHeight);
    
    for (int i = 0, y = 0; y < swapAreaHeight; y++)
    {
        for (int x = 0; x < swapAreaWidth; x++)
        {
            swapIndices.set(i++, y * columns + x);
        }
    }
    
    // Shuffle swap indices
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(swapIndices.getRawDataPointer(),
                 swapIndices.getRawDataPointer() + swapIndices.size(),
                 std::default_random_engine(seed));

    int numSwapTries = (int) jmax<int>(1, (sampleFactor * rows * columns / swapPositions.size()));
    
    for (int n = 0; n < numSwapTries; n++)
    {
        int numSwapPositions = findSwapPositionsWrap(swapIndices, swapPositions, swapAreaWidth, swapAreaHeight, rows, columns);
        doSwaps(swapPositions, numSwapPositions, inSampleItems, grid);
    }
}

int SampleItemGridSorter::findSwapPositionsWrap(Array<int> swapIndices, Array<int> swapPositions, int swapAreaWidth, int swapAreaHeight, int rows, int columns)
{
    std::random_device random;
    std::mt19937 generator(random());
    
    std::uniform_int_distribution<> distribution1(0, swapIndices.size() - swapPositions.size());
    int startIndex = (swapIndices.size() - swapPositions.size() > 0) ? distribution1(generator) : 0;
    
    std::uniform_int_distribution<> distribution2(0, rows * columns);
    int pos0 = distribution2(generator);
    
    int numSwapPositions = 0;
    for (int j = startIndex; j < swapIndices.size() && numSwapPositions < swapPositions.size(); j++)
    {
        int d = pos0 + swapIndices.getReference(j);
        int x = d % columns;
        int y = (d / columns) % rows;
        int pos = y * columns + x;

        swapPositions.set(numSwapPositions++, pos);
    }
    
    return swapPositions.size();
}

void SampleItemGridSorter::doSwaps(Array<int> swapPositions, int numSwapPositions, OwnedArray<SampleItem>& inSampleItems, Array<std::vector<float>> grid)
{
    int numValid = 0;
    
    for (int i = 0; i < numSwapPositions; i++)
    {
        int swapPosition = swapPositions.getReference(i);
        SampleItem* swappedElement = inSampleItems.getUnchecked(swapPosition);
        swappedElements.set(i, swappedElement);
        
        // Handle holes
        if (swappedElement != nullptr)
        {
            featureVectors.set(i, swappedElement->getFeatureVector());
            numValid++;
        }
        else
        {
            // Hole
            featureVectors.set(i, grid.getReference(swapPosition));
        }
        
        gridFeatureVectors.set(i, grid.getReference(swapPosition));
    }
                
    if (numValid > 0)
    {
        Array<Array<int>> distLut = calcDistLutL2Int(featureVectors, gridFeatureVectors, numSwapPositions);
        Array<int> permutation = computeAssignment(distLut, numSwapPositions);

        for (int i = 0; i < numSwapPositions; i++)
        {
            inSampleItems.set(swapPositions.getReference(permutation.getReference(i)), swappedElements.getReference(i));
        }
    }
}

Array<Array<int>> SampleItemGridSorter::calcDistLutL2Int(Array<std::vector<float>> inFeatureVectors, Array<std::vector<float>> inGridVectors, int size)
{
    float max = 0;
    
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            float val = calculateDistance(inFeatureVectors.getReference(i), inGridVectors.getReference(j));
            distLutF.getReference(i).set(j, val);
            
            if (val > max)
            {
                max = val;
            }
        }
    }
    
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            distLut.getReference(i).set(j, (int) (QUANT * distLutF.getReference(i).getReference(j) / max + 0.5));
        }
    }
        
    return distLut;
}

float SampleItemGridSorter::calculateDistance(std::vector<float> v1, std::vector<float> v2)
{
    float distance = 0.0;
    
    for (int d = 0; d < v1.size(); d++)
    {
        float dist = v1[d] - v2[d];
        distance += dist * dist;
    }
    
    return sqrt(distance);
}

Array<int> SampleItemGridSorter::computeAssignment(Array<Array<int>> matrix, int dim)
{
    int i, imin, i0, freerow;
    int j, j1, j2 = 0, endofpath = 0, last = 0, min = 0;

    Array<int> inRow;
    inRow.resize(dim);
    Array<int> inCol;
    inCol.resize(dim);
    Array<int> v;
    v.resize(dim);
    Array<int> free;
    free.resize(dim);
    Array<int> collist;
    collist.resize(dim);
    Array<int> matches;
    matches.resize(dim);
    Array<int> pred;
    pred.resize(dim);
    Array<int> d;
    d.resize(dim);

    // Skipping L53-54
    for (j = dim - 1; j >= 0; j--)
    {
        min = matrix.getReference(0).getReference(j);
        imin = 0;
        
        for (i = 1; i < dim; i++)
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
    
    for (i = 0; i < dim; i++)
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
            
            for (j = 0; j < dim; j++)
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

            for (j = 1; j < dim; j++)
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
        
        for (j = 0; j < dim; j++)
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

                for (int k = up; k < dim; k++)
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

                for (int k = up; k < dim; k++)
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
