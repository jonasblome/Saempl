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
    void addSampleItem(String forFile);
    void removeSampleItem(String forFile, bool deletePermanently);
    void setDirectory(const File& inFile);
    void switchToParentDirectory();
    
private:
    SampleDatabase& sampleDatabase;
    
};
