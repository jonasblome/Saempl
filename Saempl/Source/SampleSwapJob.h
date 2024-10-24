/*
 ==============================================================================
 
 SampleAnalysisJob.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"
#include <random>

class SampleSwapJob
:
public ThreadPoolJob
{
public:
    /**
     The sample swap job constructor.
     */
    SampleSwapJob(OwnedArray<SampleItem>& inSampleItems,
                  std::set<int> & inStartIndicesInUse,
                  std::set<int> & inSwapPositionsInUse,
                  CriticalSection & inSwapLock,
                  int inNumSwapPositions,
                  bool inApplyWrap,
                  std::vector<int> inSwapAreaIndices,
                  int inSwapAreaWidth,
                  int inSwapAreaHeight,
                  int inRows,
                  int inColumns,
                  std::vector<std::vector<float>> & inGrid);
    ~SampleSwapJob();
    
private:
    static int const QUANT = 2048;
    int const numSwapPositions;
    bool const applyWrap;
    int const swapAreaWidth;
    int const swapAreaHeight;
    int const rows;
    int const columns;
    int startIndex;
    OwnedArray<SampleItem> & sampleItems;
    std::set<int> & swapPositionsInUse;
    std::set<int> & startIndicesInUse;
    CriticalSection & swapLock;
    std::vector<SampleItem*> swappedElements;
    std::vector<int> swapPositions;
    std::vector<int> swapAreaIndices;
    std::vector<std::vector<float>> mSwappedFeatureVectors;
    std::vector<std::vector<float>> mGridVectorsAtSwapPosition;
    std::vector<std::vector<float>> mDistanceMatrix;
    std::vector<std::vector<int>> mDistanceMatrixNormalised;
    std::vector<std::vector<float>>& grid;
    
    /**
     Runs the swapping of sample items on the grid.
     */
    ThreadPoolJob::JobStatus runJob() override;
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
    int findSwapPositionsWrap(std::vector<int>& swapAreaIndices,
                              std::vector<int>& swapPositions,
                              int swapAreaWidth,
                              int swapAreaHeight,
                              int rows,
                              int columns);
    int getPosition(int columns, int rows, int sp, std::vector<int> &swapAreaIndices, int xStart, int yStart);
    
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
    int findSwapPositions(std::vector<int>& swapAreaIndices,
                          std::vector<int>& swapPositions,
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
    void doSwaps(std::vector<int>& swapPositions, int numSwapPositions, std::vector<std::vector<float>>& grid);
    /**
     Calculates a normalised distance matrix between all vectors that are in the swap positions and the current vectors in the grid.
     
     @param inFeatureVectors the feature vectors of the collection to cluster.
     @param inGridVectors the vectors in the current grid state.
     @param size the size of the grid.
     */
    std::vector<std::vector<int>> calculateNormalisedDistanceMatrix(std::vector<std::vector<float>>& inFeatureVectors,
                                                                    std::vector<std::vector<float>>& inGridVectors,
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
    std::vector<int> computeAssignment(std::vector<std::vector<int>>& matrix, int numDimensions);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleSwapJob);
};
