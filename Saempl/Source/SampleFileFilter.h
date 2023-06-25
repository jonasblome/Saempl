/*
  ==============================================================================

    SampleFileFilter.h
    Created: 10 Jun 2023 6:14:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"

class SampleFileFilter
:   public FileFilter
{
public:
    // Constructors
    SampleFileFilter(String& inDescription);
    ~SampleFileFilter();
    
    // Methods
    bool isFileSuitable (const File& file) const override;
    bool isDirectorySuitable (const File& file) const override;
    void addFilteredFilePath(SampleItem* inSampleItem);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleFileFilter)
    
    // Fields
    OwnedArray<SampleItem> mFilteredFilePaths;
    
    // Methods
    
};
