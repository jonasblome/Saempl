/*
  ==============================================================================

    SampleDatabaseTableViewModel.h
    Created: 21 May 2023 3:14:48pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "SampleDatabase.h"

class SampleDatabaseTableViewModel
{
public:
    // Constructors
    SampleDatabaseTableViewModel(SampleDatabase& inSampleDatabaseTable);
    ~SampleDatabaseTableViewModel();
    
    // Methods
    DirectoryContentsList* getDirectoryList();
    void addFile(String inFilePath);
    void removeFile(String inFilePath);
    
private:
    SampleDatabase& sampleDatabaseTable;
    
};
