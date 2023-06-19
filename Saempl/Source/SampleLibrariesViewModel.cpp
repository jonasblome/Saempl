/*
  ==============================================================================

    SampleLibrariesViewModel.cpp
    Created: 21 May 2023 3:14:48pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibrariesViewModel.h"

SampleLibrariesViewModel::SampleLibrariesViewModel(SampleLibrary& inSampleDatabaseTable)
:   sampleDatabase(inSampleDatabaseTable)
{
    
}

SampleLibrariesViewModel::~SampleLibrariesViewModel()
{
    
}

DirectoryContentsList* SampleLibrariesViewModel::getDirectoryList()
{
    return sampleDatabase.getDirectoryList();
}

/**
 Adds a file and sample item to the database
 */
void SampleLibrariesViewModel::addSampleItem(String inFilePath)
{
    File file = File(inFilePath);
    
    sampleDatabase.addSampleItem(file);
}

/**
 Removes the selected file from the current directory and removes the corresponding SampleItem from the database
 
 @param deletePermanently   determines if the item is deleted permanently (true) or just moved to the trash (false)
 */
void SampleLibrariesViewModel::removeSampleItem(String inFilePath, bool deletePermanently = false)
{
    sampleDatabase.removeSampleItem(inFilePath, deletePermanently);
}

void SampleLibrariesViewModel::refreshSampleDatabase()
{
    sampleDatabase.refreshSampleDatabase();
}
