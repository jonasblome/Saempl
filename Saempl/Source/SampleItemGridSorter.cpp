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

void SampleItemGridSorter::applySorting(OwnedArray<SampleItem> & inSampleItems, 
                                        int gridWidth,
                                        int gridHeight,
                                        bool wrapAround)
{
    int gridSize = gridWidth * gridHeight;
    int numDimensions = (int) inSampleItems.getFirst()->getFeatureVector().size();
    float weights[gridSize];
    float rad = jmax<int>(gridWidth, gridHeight) * initialRadiusFactor;
    Array<std::vector<float>> grid;
    grid.resize(gridSize);
    
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
        for (int pos = 0; pos < gridSize; pos++)
        {
            grid.set(pos, std::vector<float>(numDimensions));
            std::vector<float> & gridCell = grid.getReference(pos);
            
            if (pos < inSampleItems.size())
            {
                std::vector<float> const & featureVector = inSampleItems.getUnchecked(pos)->getFeatureVector();
                
                for (int d = 0; d < numDimensions; d++)
                {
                    gridCell.at(d) = featureVector.at(d) * weightTile;
                }
                
                weights[pos] = weightTile;
            }
            else
            {
                for (int d = 0; d < numDimensions; d++)
                {
                    gridCell.at(d) *= weightHole;
                }
                
                weights[pos] = weightTile;
            }
        }
        
        // Apply filter to map vectors
        // TODO
    
        // Loop over all iterations
            // Select a random position on the map
            // Select n random swap candidates within the radius (max(rad, (sqrt(n) - 1) / 2))
            // Find the best swapping permutation
            // Assign the input vectors to their new grid positions
        
        // Reduce the filter radius: rad = rad * radiusDecay
        rad *= radiusDecay;
    }
}
