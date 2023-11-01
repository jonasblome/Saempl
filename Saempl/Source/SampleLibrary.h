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
    void addToPalette(File const & inFile);
    void removeSampleItem(String const & inFilePath, bool deletePermanently);
    void removeFromPalette(SampleItem& inSampleItem);
    DirectoryContentsList& getDirectoryList();
    SampleFileFilter& getFileFilter();
    void changeListenerCallback(ChangeBroadcaster* inSource) override;
    void refresh();
    SampleItem* getSampleItemWithFileName(String const & inFilePath);
    void setDirectory(String inDirectoryPath);
    OwnedArray<SampleItem>& getSampleItems(SampleItemCollectionType inCollectionType);
    void applyFilter();
    String getCurrentLibraryPath();
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleLibrary)
    
    // Fields
    std::unique_ptr<SampleFileFilter> mFileFilter;
    String mCurrentLibraryPath;
    std::unique_ptr<DirectoryContentsList> mDirectoryContent;
    OwnedArray<SampleItem> mAllSampleItems;
    OwnedArray<SampleItem> mFilteredSampleItems;
    OwnedArray<SampleItem> mPaletteSampleItems;
    String mDirectoryPathToAddFilesTo;
    std::unique_ptr<SampleAnalyser> mSampleAnalyser;
    std::unique_ptr<SampleLibraryManager> mSampleLibraryManager;
    
    // Methods
    void createSampleItem(File inFile);
};
