/*
  ==============================================================================

    SampleDatabase.h
    Created: 21 May 2023 2:45:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once
#include "SampleItem.h"

class SampleDatabase
{
public:
    // Constructors
    SampleDatabase(TimeSliceThread& inThread);
    ~SampleDatabase();
    
    // Methods
    void addSampleItem(String inFilePath);
    void addSampleItem(SampleItem inItem);
    void removeSampleItem(String inFilePath);
    DirectoryContentsList* getDirectoryList();
    
private:
    // Fields
    String mSampleItemDirectory;
    std::unique_ptr<DirectoryContentsList> mDirectoryList;
    OwnedArray<SampleItem> mSampleItems;
    
    // Methods
    
};
