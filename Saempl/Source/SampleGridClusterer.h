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
#include "SampleSwapJob.h"
#include <random>
#include <limits.h>

/**
 Clusters the filtered sample items of the library according to the Fast Linear Assignment Sorting onto a 2D grid.
 */
class SampleGridClusterer
:
public ThreadWithProgressWindow,
public ChangeBroadcaster,
public ThreadPool
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
    constexpr static float const initialRadiusFactor = 0.5; // Keep <= 0.5 to not waste time
    constexpr static float const endRadius = 1.0;
    constexpr static float const weightHole = 0.01;
    constexpr static float const weightTile = 1.0;
    constexpr static float const sampleFactor = 2.0; // How often all tiles in the swap area are swapped per radius reduction
    static int const maxSwapPositions = 9;
    int const numThreads;
    std::set<int> swapPositionsInUse;
    std::set<int> startIndicesInUse;
    CriticalSection mSwapLock;
    OwnedArray<SampleItem>& sampleItems;
    std::vector<float> mFeatureWeights;
    int rows;
    int columns;
    int numSwapPositions;
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
    void copyFeatureVectorsToGrid(std::vector<std::vector<float>>& grid, int numDimensions, std::vector<float>& weights);
    /**
     Filters the grid's vectors horizontally with a low-pass filter, wrapping the grid around the edges.
     
     @param the grid of vectors to filter.
     @param rows the number of rows in the grid.
     @param columns the number of columns in the grid.
     @param numDimensions the number of dimensions in a feature vector.
     @param filterSize the length of the filter block.
     
     @returns the filtered grid vectors.
     */
    std::vector<std::vector<float>> filterHorizontallyWrap(std::vector<std::vector<float>>& inGrid,
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
    std::vector<std::vector<float>> filterVerticallyWrap(std::vector<std::vector<float>>& inGrid,
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
    std::vector<float> filterHorizontallyWrap(std::vector<float>& inWeights,
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
    std::vector<float> filterVerticallyWrap(std::vector<float>& inWeights,
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
    std::vector<std::vector<float>> filterHorizontallyMirror(std::vector<std::vector<float>>& inGrid,
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
    std::vector<std::vector<float>> filterVerticallyMirror(std::vector<std::vector<float>>& inGrid,
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
    std::vector<float> filterHorizontallyMirror(std::vector<float>& inWeights,
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
    std::vector<float> filterVerticallyMirror(std::vector<float>& inWeights,
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
    void checkRandomSwaps(int radius, std::vector<std::vector<float>>& grid, int rows, int columns);
    /**
     Calculates the radius decay dependent on the current radius.
     
     @param inRadius the current radius.
     @returns the new radius decay.
     */
    float getRadiusDecay(float inRadius);
    /**
     Sets the threads progress bar and status message.
     */
    void setProgressAndStatus(double inProgress, int64 startTime);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleGridClusterer);
};
