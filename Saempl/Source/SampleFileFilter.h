/*
  ==============================================================================

    SampleFileFilter.h
    Created: 10 Jun 2023 6:14:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class SampleFileFilter
:   public FileFilter
{
public:
    // Constructors
    SampleFileFilter(String inDescription);
    ~SampleFileFilter();
    
    // Methods
    bool isFileSuitable (const File& file) const override;
    bool isDirectorySuitable (const File& file) const override;
    void addFilteredFilePath(String inFilePath);
    
private:
    // Fields
    OwnedArray<String> mFilteredFilePaths;
    
    // Methods
    
};
