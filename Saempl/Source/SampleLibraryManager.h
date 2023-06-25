/*
  ==============================================================================

    SampleLibraryManager.h
    Created: 10 Jun 2023 6:16:05pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"
#include "BlomeHelperFunctions.h"

class SampleLibraryManager
{
public:
    // Constructors
    SampleLibraryManager();
    ~SampleLibraryManager();
    
    // Methods
    void updateSampleLibraryFile(String& inFilePath, OwnedArray<SampleItem>* inSampleItem);
    void loadSampleLibraryFile(String& inFilePath, OwnedArray<SampleItem>* inSampleItems);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleLibraryManager)
    
    // Fields
    String mLibraryFilesDirectoryPath;
    
    // Methods
    String getLibraryFilesDirectoryPath();
    
};
