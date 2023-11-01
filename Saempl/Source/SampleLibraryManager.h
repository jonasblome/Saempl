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
#include "BlomeHelpers.h"

/**
 Handles updating and creating of directory meta-analysis files.
 
 Writes information on all files in a sample library directory to analysis files and reads them.
 */
class SampleLibraryManager
{
public:
    // Constructors
    SampleLibraryManager();
    ~SampleLibraryManager();
    
    // Methods
    /**
     Adds meta-information of all sample items to an analysis file and updates their information if needed.
     
     @param inFilePath the path to the library directory.
     @param inSampleItems the collection of sample items from the library.
     */
    void updateSampleLibraryFile(String& inFilePath, OwnedArray<SampleItem>& inSampleItems);
    /**
     Loads meta-information of analysis file as sample items collection.
     
     @param the path to the library analysis file.
     @param the collection of sample items from the library.
     */
    void loadSampleLibraryFile(String& inFilePath, OwnedArray<SampleItem>& inSampleItems);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleLibraryManager)
    
    String mLibraryFilesDirectoryPath;
    
    String getLibraryFilesDirectoryPath();
};
