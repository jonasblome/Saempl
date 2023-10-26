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

class SampleLibraryViewModel
{
public:
    // Constructors
    SampleLibraryViewModel(SampleLibrary& inSampleLibrary);
    ~SampleLibraryViewModel();
    
    // Methods
    DirectoryContentsList* getDirectoryList();
    OwnedArray<SampleItem>* getSampleItems();
    void addSampleItem(String const & inFilePath);
    void removeSampleItem(String const & inFilePath, bool deletePermanently);
    void refreshSampleLibrary();
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleLibraryViewModel)
    
    // Fields
    SampleLibrary& sampleLibrary;
    
    // Methods
    
};
