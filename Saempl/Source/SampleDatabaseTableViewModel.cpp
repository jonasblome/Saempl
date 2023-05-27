/*
  ==============================================================================

    SampleDatabaseTableViewModel.cpp
    Created: 21 May 2023 3:14:48pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleDatabaseTableViewModel.h"

SampleDatabaseTableViewModel::SampleDatabaseTableViewModel(TimeSliceThread& inThread)
:   mSampleDatabaseTable(inThread)
{
    
}

SampleDatabaseTableViewModel::~SampleDatabaseTableViewModel()
{
    
}

DirectoryContentsList* SampleDatabaseTableViewModel::getDirectoryList()
{
    return mSampleDatabaseTable.getDirectoryList();
}
