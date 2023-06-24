/*
  ==============================================================================

    SampleLibrariesViewModel.cpp
    Created: 21 May 2023 3:14:48pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibraryViewModel.h"

SampleLibrariesViewModel::SampleLibrariesViewModel(SampleLibrary& inSampleDatabaseTable)
:   sampleLibrary(inSampleDatabaseTable)
{
    
}

SampleLibrariesViewModel::~SampleLibrariesViewModel()
{
    
}

DirectoryContentsList* SampleLibrariesViewModel::getDirectoryList()
{
    return sampleLibrary.getDirectoryList();
}

/**
 Adds a file and sample item to the database
 */
void SampleLibrariesViewModel::addSampleItem(String inFilePath)
{
    File file = File(inFilePath);
    
    sampleLibrary.addSampleItem(file);
}

/**
 Removes the selected file from the current directory and removes the corresponding SampleItem from the database
 
 @param deletePermanently   determines if the item is deleted permanently (true) or just moved to the trash (false)
 */
void SampleLibrariesViewModel::removeSampleItem(String inFilePath, bool deletePermanently = false)
{
    sampleLibrary.removeSampleItem(inFilePath, deletePermanently);
}

void SampleLibrariesViewModel::refreshSampleLibrary()
{
    sampleLibrary.refreshSampleLibrary();
}
