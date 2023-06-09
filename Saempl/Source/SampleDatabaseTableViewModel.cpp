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
    
    if (file.isDirectory() || isSupportedAudioFileFormat(file.getFileExtension()))
    {
        sampleDatabase.addSampleItem(file);
    }
}

void SampleDatabaseTableViewModel::removeSampleItem(String forFile)
{
    sampleDatabase.removeSampleItem(forFile);
}

void SampleDatabaseTableViewModel::moveSampleItemToTrash(String forFile)
{
    sampleDatabase.moveSampleItemToTrash(forFile);
}

void SampleDatabaseTableViewModel::setDirectory(const File& inFile)
{
    sampleDatabase.setDirectory(inFile);
}

void SampleDatabaseTableViewModel::setToParentDirectory()
{
    sampleDatabase.setToParentDirectory();
}
