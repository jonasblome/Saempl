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
    void applySorting(OwnedArray<SampleItem> & inSampleItems, int gridWidth, int gridHeight, bool wrapAround);
    
private:
    constexpr static const float initialRadiusFactor = 0.5;
    constexpr static const float radiusDecay = 0.9;
    constexpr static const float endRadius = 1.0;
    constexpr static const float weightHole = 0.01;
    constexpr static const float weightTile = 1.0;
    constexpr static const float sampleFactor = 1.0;
    static const int maxSwapPositions = 9;
    
};
