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

class SampleDatabase
:   ChangeListener
{
public:
    // Constructors
    SampleDatabase(TimeSliceThread& inThread);
    ~SampleDatabase();
    
    // Methods
    void addSampleItem(File inFile);
    void addSampleItem(SampleItem inItem);
    void removeSampleItem(String inFilePath);
    void moveSampleItemToTrash(String inFilePath);
    DirectoryContentsList* getDirectoryList();
    void setDirectory(const File& inFile);
    void setToParentDirectory();
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
private:
    // Fields
    WildcardFileFilter mDirectoryFilter;
    String mSampleItemDirectoryPath;
    std::unique_ptr<DirectoryContentsList> mDirectoryList;
    OwnedArray<SampleItem> mSampleItems;
    String mDirectoryPathToAddFilesTo;
    
    // Methods
    
};
