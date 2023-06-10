/*
  ==============================================================================

    SampleDatabase.h
    Created: 21 May 2023 2:45:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once
#include "SampleItem.h"
#include "BlomeHelperFunctions.h"
#include "SampleAnalyser.h"

class SampleDatabase
:   ChangeListener
{
public:
    // Constructors
    SampleDatabase(TimeSliceThread& inThread);
    ~SampleDatabase();
    
    // Methods
    void addSampleItem(File inFile);
    void loadSampleItem(SampleItem inItem);
    void removeSampleItem(String inFilePath, bool deletePermanently);
    DirectoryContentsList* getDirectoryList();
    void setDirectory(const File& inFile);
    void switchToParentDirectory();
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
private:
    // Fields
    WildcardFileFilter mDirectoryFilter;
    String mSampleItemDirectoryPath;
    std::unique_ptr<DirectoryContentsList> mDirectoryList;
    OwnedArray<SampleItem> mSampleItems;
    String mDirectoryPathToAddFilesTo;
    std::unique_ptr<SampleAnalyser> mSampleAnalyser;
    
    // Methods
    SampleItem* getSampleItemWithFilePath(String inFilePath);
};
