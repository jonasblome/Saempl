/*
 ==============================================================================
 
 SampleGridClusterer.h
 Author:  Jonas Blome
 
 Translated and adapted code from the paper
 "Improved Evaluation and Generation of Grid Layouts using Distance Preservation Quality and Linear Assignment Sorting"
 by Kai Barthel, Nico Hezel, Klaus Jung, and Konstantin Schall.
 @HTW Berlin, Visual Computing Group, Germany
 https://visual-computing.com/
 The code was originally written by Nico Hezel, Konstantin Schall and Kai Barthel.
 
 ==============================================================================
 */

#pragma once

#include "SampleItem.h"
#include <random>
#include <limits.h>

/**
 Clusters the filtered sample items of the library according to the Fast Linear Assignment Sorting onto a 2D grid.
 */
class SampleGridClusterer
:
public ThreadWithProgressWindow,
public ChangeBroadcaster
{
public:
    SampleGridClusterer(OwnedArray<SampleItem>& inSampleItems);
    ~SampleGridClusterer();
    /**
     Applies the clustering algorithm to the given grid of sample items.
     
     @param rows the number of rows in the grid.
     @param columns the number of columns in the grid.
     @param doWrap whether the clustering should wrap around the edges of the grid.
     */
    void applyClustering(int rows, int columns, bool doWrap);
    /**
     Sets the collection feature weights.
     */
    void setFeatureWeights(std::vector<float> inFeatureWeights);
    
private:
    constexpr static float const initialRadiusFactor = 0.5;
    constexpr static float const radiusDecay = 0.975;
    constexpr static float const endRadius = 1.0;
    constexpr static float const weightHole = 0.01;
    constexpr static float const weightTile = 1.0;
    constexpr static float const sampleFactor = 2.0;
    static int const maxSwapPositions = 9;
    static int const QUANT = 2048;
    Array<int> swapPositions;
    Array<SampleItem*> swappedElements;
    Array<std::vector<float>> mFeatureVectors;
    Array<std::vector<float>> mGridVectors;
    Array<Array<float>> mDistanceMatrix;
    Array<Array<int>> mDistanceMatrixNormalised;
    OwnedArray<SampleItem>& sampleItems;
    std::vector<float> mFeatureWeights;
    int rows;
    int columns;
    bool applyWrap;
    bool featureWeightsChanged = true;
    
    /**
     Runs the clustering of the sample items while setting the progress for the progress bar.
     */
    void run() override;
    void threadComplete(bool userPressedCancel) override;
    /**
     Copies the feature vectors from the sample item collection to the grid and multiplies each dimension with a weight.
     
     @param grid the grid of vectors to cluster.
     @param numDimensions the number of dimensions in a feature vector.
     @param weights the weights for the grid positions.
     */
    void copyFeatureVectorsToGrid(Array<std::vector<float>>& grid, int numDimensions, Array<float>& weights);
    /**
     Filters the grid's vectors horizontally with a low-pass filter, wrapping the grid around the edges.
     
     @param the grid of vectors to filter.
     @param rows the number of rows in the grid.
     @param columns the number of columns in the grid.
     @param numDimensions the number of dimensions in a feature vector.
     @param filterSize the length of the filter block.
     
     @returns the filtered grid vectors.
     */
    Array<std::vector<float>> filterHorizontallyWrap(Array<std::vector<float>>& inGrid,
                                                     int rows,
                                                     int columns,
                                                     int numDimensions,
                                                     int filterSize);
    /**
     Filters the grid's vectors vertically with a low-pass filter, wrapping the grid around the edges.
     
     @param the grid of vectors to filter.
     @param rows the number of rows in the grid.
     @param columns the number of columns in the grid.
     @param numDimensions the number of dimensions in a feature vector.
     @param filterSize the length of the filter block.
     
     @returns the filtered grid vectors.
     */
    Array<std::vector<float>> filterVerticallyWrap(Array<std::vector<float>>& inGrid,
                                                   int rows,
                                                   int columns,
                                                   int numDimensions,
                                                   int filterSize);
    /**
     Filters the grid's weights horizontally with a low-pass filter, wrapping the grid around the edges.
     
     @param the grid of vectors to filter.
     @param rows the number of rows in the grid.
     @param columns the number of columns in the grid.
     @param filterSize the length of the filter block.
     
     @returns the filtered weights.
     */
    Array<float> filterHorizontallyWrap(Array<float>& inWeights,
                                        int rows,
                                        int columns,
                                        int filterSize);
    /**
     Filters the grid's weights vertically with a low-pass filter, wrapping the grid around the edges.
     
     @param the grid of vectors to filter.
     @param rows the number of rows in the grid.
     @param columns the number of columns in the grid.
     @param filterSize the length of the filter block.
     
     @returns the filtered weights.
     */
    Array<float> filterVerticallyWrap(Array<float>& inWeights,
                                      int rows,
                                      int columns,
                                      int filterSize);
    /**
     Filters the grid's vectors horizontally with a low-pass filter, mirroring the grid at the edges.
     
     @param the grid of vectors to filter.
     @param rows the number of rows in the grid.
     @param columns the number of columns in the grid.
     @param numDimensions the number of dimensions in a feature vector.
     @param filterSize the length of the filter block.
     
     @returns the filtered grid vectors.
     */
    Array<std::vector<float>> filterHorizontallyMirror(Array<std::vector<float>>& inGrid,
                                                       int rows,
                                                       int columns,
                                                       int numDimensions,
                                                       int filterSize);
    /**
     Filters the grid's vectors vertically with a low-pass filter, mirroring the grid at the edges.
     
     @param the grid of vectors to filter.
     @param rows the number of rows in the grid.
     @param columns the number of columns in the grid.
     @param numDimensions the number of dimensions in a feature vector.
     @param filterSize the length of the filter block.
     
     @returns the filtered grid vectors.
     */
    Array<std::vector<float>> filterVerticallyMirror(Array<std::vector<float>>& inGrid,
                                                     int rows,
                                                     int columns,
                                                     int numDimensions,
                                                     int filterSize);
    /**
     Filters the grid's weights horizontally with a low-pass filter, mirroring the grid at the edges.
     
     @param the grid of vectors to filter.
     @param rows the number of rows in the grid.
     @param columns the number of columns in the grid.
     @param filterSize the length of the filter block.
     
     @returns the filtered weights.
     */
    Array<float> filterHorizontallyMirror(Array<float>& inWeights,
                                          int rows,
                                          int columns,
                                          int filterSize);
    /**
     Filters the grid's weights vertically with a low-pass filter, mirroring the grid at the edges.
     
     @param the grid of vectors to filter.
     @param rows the number of rows in the grid.
     @param columns the number of columns in the grid.
     @param filterSize the length of the filter block.
     
     @returns the filtered weights.
     */
    Array<float> filterVerticallyMirror(Array<float>& inWeights,
                                        int rows,
                                        int columns,
                                        int filterSize);
    /**
     Defines an area with the given radius, and checks random swaps within that area around a random sample for optimal relative distances.
     When the optimal permutation is found, the swapping of those positions is performed.
     
     @param radius the radius that defines the swap area.
     @param grid the grid of vectors to cluster.
     @param rows the number of rows in the grid.
     @param columns the number of columns in the grid.
     */
    void checkRandomSwaps(int radius, Array<std::vector<float>>& grid, int rows, int columns);
    /**
     Chooses a random position on the grid and finds a set of random positions within a given area around that initial position,
     while wrapping the grid around the edges.
     
     @param swapAreaIndices the indices of the predefined swap area.
     @param swapPositions the random positions to swap.
     @param swapAreaWidth the width of the predefined swap area.
     @param swapAreaHeight the height of the predefined swap area.
     @param grid the grid of vectors to cluster.
     @param rows the number of rows in the grid.
     
     @returns the amound of swaps.
     */
    int findSwapPositionsWrap(Array<int>& swapAreaIndices,
                              Array<int>& swapPositions,
                              int swapAreaWidth,
                              int swapAreaHeight,
                              int rows,
                              int columns);
    /**
     Chooses a random position on the grid and finds a set of random positions within a given area around that initial position,
     while mirroring the grid at the edges.
     
     @param swapAreaIndices the indices of the predefined swap area.
     @param swapPositions the random positions to swap.
     @param swapAreaWidth the width of the predefined swap area.
     @param swapAreaHeight the height of the predefined swap area.
     @param grid the grid of vectors to cluster.
     @param rows the number of rows in the grid.
     
     @returns the amound of swaps.
     */
    int findSwapPositions(Array<int>& swapAreaIndices,
                          Array<int>& swapPositions,
                          int swapAreaWidth,
                          int swapAreaHeight,
                          int rows,
                          int columns);
    /**
     Finds the best permutation for a given number of swaps to the grid positions and the performs the optimal permutation's swap.
     
     @param swapPosition  the indices of the grid positions to swap.
     @param numSwapPositions the number of swaps to perform.
     @param grid the grid of vectors to cluster.
     */
    void doSwaps(Array<int>& swapPositions, int numSwapPositions, Array<std::vector<float>>& grid);
    /**
     Calculates a normalised distance matrix between all vectors that are in the swap positions and the current vectors in the grid.
     
     @param inFeatureVectors the feature vectors of the collection to cluster.
     @param inGridVectors the vectors in the current grid state.
     @param size the size of the grid.
     */
    Array<Array<int>> calculateNormalisedDistanceMatrix(Array<std::vector<float>>& inFeatureVectors,
                                                        Array<std::vector<float>>& inGridVectors,
                                                        int size);
    /**
     Calculates the euclidean distance between the two vectors.
     @param v1 the first vector.
     @param v2 the second vector.
     */
    float calculateDistance(std::vector<float> v1, std::vector<float> v2);
    /**
     Computes the optimal permutation of vectors that minimises the sum of distances between the vectors.
     
     @param matrix the distance matrix for the vectors.
     @param numDimensions the number of dimensions in a feature vector.
     */
    Array<int> computeAssignment(Array<Array<int>>& matrix, int numDimensions);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleGridClusterer);
};
