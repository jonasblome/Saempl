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
#include "SampleFileManager.h"

class SampleDatabase
:   ChangeListener
{
public:
    // Constructors
    SampleDatabase(TimeSliceThread& inThread);
    ~SampleDatabase();
    
    // Methods
    void addSampleItem(File inFile);
    void removeSampleItem(String inFilePath, bool deletePermanently);
    DirectoryContentsList* getDirectoryList();
    void setDirectory(const File& inFile);
    void switchToParentDirectory();
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void setFileFilter();
    void loadSampleFilesFromDirectory();
    
private:
    // Fields
    std::unique_ptr<FileFilter> mDirectoryFilter;
    String mSampleItemDirectoryPath;
    std::unique_ptr<DirectoryContentsList> mDirectoryList;
    OwnedArray<SampleItem> mSampleItems;
    String mDirectoryPathToAddFilesTo;
    std::unique_ptr<SampleAnalyser> mSampleAnalyser;
    std::unique_ptr<SampleFileManager> mSampleFileManager;
    
    // Methods
    SampleItem* getSampleItemWithFilePath(String inFilePath);
};
