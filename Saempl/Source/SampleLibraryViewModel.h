/*
  ==============================================================================

    SampleItemLibraryViewModel.h
    Created: 21 May 2023 3:14:48pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "SampleLibrary.h"
#include "SampleAnalyser.h"

class SampleLibraryViewModel
{
public:
    // Constructors
    SampleLibraryViewModel(SampleLibrary& inSampleItemLibrary);
    ~SampleLibraryViewModel();
    
    // Methods
    DirectoryContentsList* getDirectoryList();
    void addSampleItem(String inFilePath);
    void removeSampleItem(String inFilePath, bool deletePermanently);
    void refreshSampleDatabase();
    OwnedArray<SampleFilterCondition>* getFilterConditions();
    
private:
    // Fields
    SampleLibrary& sampleItemLibrary;
    
    // Methods
    
};
