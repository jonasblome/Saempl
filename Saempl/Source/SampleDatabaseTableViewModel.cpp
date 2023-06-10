/*
  ==============================================================================

    SampleDatabaseTableViewModel.cpp
    Created: 21 May 2023 3:14:48pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleDatabaseTableViewModel.h"

SampleDatabaseTableViewModel::SampleDatabaseTableViewModel(SampleDatabase& inSampleDatabaseTable)
:   sampleDatabase(inSampleDatabaseTable)
{
    
}

SampleDatabaseTableViewModel::~SampleDatabaseTableViewModel()
{
    
}

DirectoryContentsList* SampleDatabaseTableViewModel::getDirectoryList()
{
    return sampleDatabase.getDirectoryList();
}

/**
 Adds a file and sample item to the database
 */
void SampleDatabaseTableViewModel::addSampleItem(String inFilePath)
{
    File file = File(inFilePath);
    
    sampleDatabase.addSampleItem(file);
}

/**
 Removes the selected file from the current directory and removes the corresponding SampleItem from the database
 
 @param deletePermanently   determines if the item is deleted permanently (true) or just moved to the trash (false)
 */
void SampleDatabaseTableViewModel::removeSampleItem(String inFilePath, bool deletePermanently = false)
{
    sampleDatabase.removeSampleItem(inFilePath, deletePermanently);
}

void SampleDatabaseTableViewModel::setDirectory(const File& inFile)
{
    sampleDatabase.setDirectory(inFile);
}

/**
 Sets the directory to the parent directory of the current one
 */
void SampleDatabaseTableViewModel::switchToParentDirectory()
{
    sampleDatabase.switchToParentDirectory();
}
