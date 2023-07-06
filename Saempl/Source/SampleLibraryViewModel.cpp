/*
  ==============================================================================

    SampleLibrariesViewModel.cpp
    Created: 21 May 2023 3:14:48pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibraryViewModel.h"

SampleLibraryViewModel::SampleLibraryViewModel(SampleLibrary& inSampleLibrary)
:   sampleLibrary(inSampleLibrary)
{
    
}

SampleLibraryViewModel::~SampleLibraryViewModel()
{
    
}

DirectoryContentsList* SampleLibraryViewModel::getDirectoryList()
{
    return sampleLibrary.getDirectoryList();
}

OwnedArray<SampleItem>* SampleLibraryViewModel::getSampleItems()
{
    return sampleLibrary.getSampleItems();
}

/**
 Adds a file and sample item to the database
 */
void SampleLibraryViewModel::addSampleItem(const String& inFilePath)
{
    File file = File(inFilePath);
    
    sampleLibrary.addSampleItem(file);
}

/**
 Removes the selected file from the current directory and removes the corresponding SampleItem from the database
 
 @param deletePermanently   determines if the item is deleted permanently (true) or just moved to the trash (false)
 */
void SampleLibraryViewModel::removeSampleItem(const String& inFilePath, bool deletePermanently = false)
{
    sampleLibrary.removeSampleItem(inFilePath, deletePermanently);
}

void SampleLibraryViewModel::refreshSampleLibrary()
{
    sampleLibrary.refresh();
}
