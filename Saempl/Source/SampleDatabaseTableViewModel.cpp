/*
  ==============================================================================

    SampleDatabaseTableViewModel.cpp
    Created: 21 May 2023 3:14:48pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleDatabaseTableViewModel.h"

SampleDatabaseTableViewModel::SampleDatabaseTableViewModel(SampleDatabase& inSampleDatabaseTable)
:   sampleDatabaseTable(inSampleDatabaseTable)
{
    
}

SampleDatabaseTableViewModel::~SampleDatabaseTableViewModel()
{
    
}

DirectoryContentsList* SampleDatabaseTableViewModel::getDirectoryList()
{
    return sampleDatabaseTable.getDirectoryList();
}

void SampleDatabaseTableViewModel::addFile(String inFilePath)
{
    sampleDatabaseTable.addSampleItem(inFilePath);
}

void SampleDatabaseTableViewModel::removeFile(String inFilePath)
{
    sampleDatabaseTable.removeSampleItem(inFilePath);
}
