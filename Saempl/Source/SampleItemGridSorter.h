/*
  ==============================================================================

    SampleItemGridSorter.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleItem.h"
#include <random>

/**
 Sorts the filtered sample items of the library according to the Fast Linear Assignment Sorting onto a 2D grid.
 */
class SampleItemGridSorter
{
public:
    SampleItemGridSorter();
    ~SampleItemGridSorter();
    /**
     Applies the sorting algorithm to the given grid of sample items.
     */
    void applySorting(OwnedArray<SampleItem> & inSampleItems, int gridWidth, int gridHeight);
    void copyFeatureVectorsToGrid(Array<std::vector<float>>& grid, OwnedArray<SampleItem>& inSampleItems, int numDimensions, Array<float> weights);
    Array<std::vector<float>> filterHorizontally(Array<std::vector<float>>& input, int rows, int columns, int dims, int filterSize);
    Array<std::vector<float>> filterVertically(Array<std::vector<float>>& input, int rows, int columns, int dims, int filterSize);
    Array<float> filterHorizontally(Array<float>& input, int rows, int columns, int filterSize);
    Array<float> filterVertically(Array<float>& input, int rows, int columns, int filterSize);
    void checkRandomSwaps(int radius, OwnedArray<SampleItem>& inSampleItems, Array<std::vector<float>> grid, int rows, int columns);
    int findSwapPositionsWrap(Array<int> swapIndices, Array<int> swapPositions, int swapAreaWidth, int swapAreaHeight, int rows, int columns);
    void doSwaps(Array<int> swapPositions, int numSwapPositions, OwnedArray<SampleItem>& inSampleItems, Array<std::vector<float>> grid);
    Array<Array<int>> calcDistLutL2Int(Array<std::vector<float>> fv, Array<std::vector<float>> mv, int size);
    float calculateDistance(std::vector<float> v1, std::vector<float> v2);
    Array<int> computeAssignment(Array<Array<int>> matrix, int dim);

private:
    constexpr static const float initialRadiusFactor = 0.5;
    constexpr static const float radiusDecay = 0.9;
    constexpr static const float endRadius = 1.0;
    constexpr static const float weightHole = 0.01;
    constexpr static const float weightTile = 1.0;
    constexpr static const float sampleFactor = 1.0;
    static const int maxSwapPositions = 9;
    static const int QUANT = 2048;
    Array<int> swapPositions;
    Array<SampleItem*> swappedElements;
    Array<std::vector<float>> featureVectors;
    Array<std::vector<float>> gridFeatureVectors;
    Array<Array<int>> distLut;
    Array<Array<float>> distLutF;
};
