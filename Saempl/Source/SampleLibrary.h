/*
  ==============================================================================

    SampleItemLibrary.h
    Created: 21 May 2023 2:45:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once
#include "SampleItem.h"
#include "BlomeHelperFunctions.h"
#include "SampleAnalyser.h"
#include "SampleManager.h"
#include "SampleFilter.h"
#include "SampleFilterCondition.h"

class SampleLibrary
:   ChangeListener
{
public:
    // Constructors
    SampleLibrary(TimeSliceThread& inThread);
    ~SampleLibrary();
    
    // Methods
    void addSampleItem(File inFile);
    void removeSampleItem(String inFilePath, bool deletePermanently);
    DirectoryContentsList* getDirectoryList();
    void changeListenerCallback(ChangeBroadcaster* inSource) override;
    void setFileFilter();
    void refreshSampleDatabase();
    SampleItem* getSampleItemWithFilePath(String inFilePath);
    void addFilterCondition(SampleFilterCondition* inFilterCondition);
    void removeFilterCondition(SampleFilterCondition* inFilterCondition);
    OwnedArray<SampleFilterCondition>* getFilterConditions();
    
private:
    // Fields
    std::unique_ptr<SampleFilter> mDirectoryFilter;
    OwnedArray<SampleFilterCondition> mFilterConditions;
    std::unique_ptr<DirectoryContentsList> mDirectory;
    String mSampleItemDirectoryPath;
    std::unique_ptr<DirectoryContentsList> mDirectoryContent;
    OwnedArray<SampleItem> mSampleItems;
    String mDirectoryPathToAddFilesTo;
    std::unique_ptr<SampleAnalyser> mSampleAnalyser;
    std::unique_ptr<SampleManager> mSampleItemManager;
    
    // Methods
    void createSampleItem(File inFile);
    void updateDirectoryFilter();
};
