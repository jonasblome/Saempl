/*
  ==============================================================================

    SampleItemFilter.h
    Created: 10 Jun 2023 6:14:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class SampleFilter
:   public FileFilter
{
public:
    // Constructors
    SampleFilter(String inDescription);
    ~SampleFilter();
    
    // Methods
    bool isFileSuitable (const File& file) const override;
    bool isDirectorySuitable (const File& file) const override;
    void addFilteredFilePath(String inFilePath);
    void clear();
    
private:
    // Fields
    OwnedArray<String> mFilteredFilePaths;
    
    // Methods
    
};
