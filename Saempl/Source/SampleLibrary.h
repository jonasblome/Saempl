/*
  ==============================================================================

    SampleLibrary.h
    Created: 21 May 2023 2:45:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleItem.h"
#include "BlomeHelpers.h"
#include "SampleAnalyser.h"
#include "SampleLibraryManager.h"
#include "SampleFileFilter.h"
#include "SampleFileFilterRuleTitle.h"
#include "SampleFileFilterRuleLength.h"

class SampleLibrary
:   ChangeListener
{
public:
    // Constructors
    SampleLibrary(TimeSliceThread& inThread);
    ~SampleLibrary();
    
    // Methods
    void addToSampleItems(File const & inFile);
    void removeSampleItem(String const & inFilePath, bool deletePermanently);
    DirectoryContentsList& getDirectoryList();
    SampleFileFilter& getFileFilter();
    void changeListenerCallback(ChangeBroadcaster* inSource) override;
    void refresh();
    SampleItem* getSampleItemWithFilePath(String const & inFilePath);
    void setDirectory(String inDirectoryPath);
    OwnedArray<SampleItem>& getFilteredSampleItems();
    void applyFilter();
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleLibrary)
    
    // Fields
    std::unique_ptr<SampleFileFilter> mLibraryFilter;
    String mCurrentLibraryPath;
    std::unique_ptr<DirectoryContentsList> mDirectoryContent;
    OwnedArray<SampleItem> mSampleItems;
    OwnedArray<SampleItem> mFilteredSampleItems;
    String mDirectoryPathToAddFilesTo;
    std::unique_ptr<SampleAnalyser> mSampleAnalyser;
    std::unique_ptr<SampleLibraryManager> mSampleLibraryManager;
    
    // Methods
    void createSampleItem(File inFile);
};
