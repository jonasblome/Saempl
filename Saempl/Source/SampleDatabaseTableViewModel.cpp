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

void SampleDatabaseTableViewModel::addSampleItem(String forFile)
{
    File file = File(forFile);
    
    sampleDatabase.addSampleItem(file);
}

void SampleDatabaseTableViewModel::removeSampleItem(String forFile, bool deletePermanently)
{
    sampleDatabase.removeSampleItem(forFile, deletePermanently);
}

void SampleDatabaseTableViewModel::setDirectory(const File& inFile)
{
    sampleDatabase.setDirectory(inFile);
}

void SampleDatabaseTableViewModel::switchToParentDirectory()
{
    sampleDatabase.switchToParentDirectory();
}
