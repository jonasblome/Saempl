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
#include "SampleAnalyser.h"

class SampleDatabaseTableViewModel
{
public:
    // Constructors
    SampleDatabaseTableViewModel(SampleDatabase& inSampleDatabase);
    ~SampleDatabaseTableViewModel();
    
    // Methods
    DirectoryContentsList* getDirectoryList();
    void addSampleItem(String inFilePath);
    void removeSampleItem(String inFilePath, bool deletePermanently);
    void setDirectory(const File& inFile);
    void switchToParentDirectory();
    
private:
    // Fields
    SampleDatabase& sampleDatabase;
    
    // Methods
    
};
