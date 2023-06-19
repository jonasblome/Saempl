/*
  ==============================================================================

    SampleItemLibraryViewModel.cpp
    Created: 21 May 2023 3:14:48pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibraryViewModel.h"

SampleLibraryViewModel::SampleLibraryViewModel(SampleLibrary& inSampleItemLibrary)
:   sampleItemLibrary(inSampleItemLibrary)
{
    
}

SampleLibraryViewModel::~SampleLibraryViewModel()
{
    
}

DirectoryContentsList* SampleLibraryViewModel::getDirectoryList()
{
    return sampleItemLibrary.getDirectoryList();
}

/**
 Adds a file and sample item to the database
 */
void SampleLibraryViewModel::addSampleItem(String inFilePath)
{
    File file = File(inFilePath);
    
    sampleItemLibrary.addSampleItem(file);
}

/**
 Removes the selected file from the current directory and removes the corresponding SampleItem from the database
 
 @param deletePermanently   determines if the item is deleted permanently (true) or just moved to the trash (false)
 */
void SampleLibraryViewModel::removeSampleItem(String inFilePath, bool deletePermanently = false)
{
    sampleItemLibrary.removeSampleItem(inFilePath, deletePermanently);
}

void SampleLibraryViewModel::refreshSampleDatabase()
{
    sampleItemLibrary.refreshSampleDatabase();
}

OwnedArray<SampleFilterCondition>* SampleLibraryViewModel::getFilterConditions()
{
    return sampleItemLibrary.getFilterConditions();
}
