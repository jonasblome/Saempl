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
:
public ThreadWithProgressWindow,
public ChangeBroadcaster
{
public:
    SampleItemGridSorter(OwnedArray<SampleItem>& inSampleItems);
    ~SampleItemGridSorter();
    /**
     Applies the sorting algorithm to the given grid of sample items.
     */
    void applySorting(int rows, int columns, bool doWrap);
    
private:
    constexpr static float const initialRadiusFactor = 0.5;
    constexpr static float const radiusDecay = 0.97;
    constexpr static float const endRadius = 1.0;
    constexpr static float const weightHole = 0.01;
    constexpr static float const weightTile = 1.0;
    constexpr static float const sampleFactor = 1.0;
    static int const maxSwapPositions = 9;
    static int const QUANT = 2048;
    Array<int> swapPositions;
    Array<SampleItem*> swappedElements;
    Array<std::vector<float>> featureVectors;
    Array<std::vector<float>> gridFeatureVectors;
    Array<Array<int>> distanceMatrixNormalised;
    Array<Array<float>> distanceMatrix;
    OwnedArray<SampleItem>& sampleItems;
    int rows;
    int columns;
    bool applyWrap;
    
    /**
     Runs the sorting of the sample items while setting the progress for the progress bar.
     */
    void run() override;
    void threadComplete(bool userPressedCancel) override;
    void copyFeatureVectorsToGrid(Array<std::vector<float>>& grid, int numDimensions, Array<float>& weights);
    Array<std::vector<float>> filterHorizontallyWrap(Array<std::vector<float>>& inGrid, 
                                                     int rows,
                                                     int columns,
                                                     int numDimensions,
                                                     int filterSize);
    Array<std::vector<float>> filterVerticallyWrap(Array<std::vector<float>>& inGrid, 
                                                   int rows,
                                                   int columns,
                                                   int numDimensions,
                                                   int filterSize);
    Array<float> filterHorizontallyWrap(Array<float>& inWeights, int rows, int columns, int filterSize);
    Array<float> filterVerticallyWrap(Array<float>& inWeights, int rows, int columns, int filterSize);
    Array<std::vector<float>> filterHorizontallyMirror(Array<std::vector<float>>& inGrid, 
                                                       int rows,
                                                       int columns,
                                                       int numDimensions,
                                                       int filterSize);
    Array<std::vector<float>> filterVerticallyMirror(Array<std::vector<float>>& inGrid, 
                                                     int rows,
                                                     int columns,
                                                     int numDimensions,
                                                     int filterSize);
    Array<float> filterHorizontallyMirror(Array<float>& inWeights, int rows, int columns, int filterSize);
    Array<float> filterVerticallyMirror(Array<float>& inWeights, int rows, int columns, int filterSize);
    void checkRandomSwaps(int radius, Array<std::vector<float>>& grid, int rows, int columns);
    int findSwapPositionsWrap(Array<int>& swapIndices, 
                              Array<int>& swapPositions,
                              int swapAreaWidth,
                              int swapAreaHeight,
                              int rows,
                              int columns);
    int findSwapPositions(Array<int>& swapIndices, 
                          Array<int>& swapPositions,
                          int swapAreaWidth,
                          int swapAreaHeight,
                          int rows,
                          int columns);
    void doSwaps(Array<int>& swapPositions, int numSwapPositions, Array<std::vector<float>>& grid);
    Array<Array<int>> calculateNormalisedDistanceMatrix(Array<std::vector<float>>& inFeatureVectors, 
                                                        Array<std::vector<float>>& inGridVectors,
                                                        int size);
    float calculateDistance(std::vector<float> v1, std::vector<float> v2);
    Array<int> computeAssignment(Array<Array<int>>& matrix, int numDimensions);
};
