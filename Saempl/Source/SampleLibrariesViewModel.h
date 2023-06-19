/*
  ==============================================================================

    SampleLibrariesViewModel.h
    Created: 21 May 2023 3:14:48pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "SampleLibrary.h"
#include "SampleAnalyser.h"

class SampleLibrariesViewModel
{
public:
    // Constructors
    SampleLibrariesViewModel(SampleLibrary& inSampleDatabase);
    ~SampleLibrariesViewModel();
    
    // Methods
    DirectoryContentsList* getDirectoryList();
    void addSampleItem(String inFilePath);
    void removeSampleItem(String inFilePath, bool deletePermanently);
    void refreshSampleDatabase();
    
private:
    // Fields
    SampleLibrary& sampleDatabase;
    
    // Methods
    
};
