/*
  ==============================================================================

    SampleDatabaseTable.cpp
    Created: 21 May 2023 3:09:08pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleDatabaseTable.h"

SampleDatabaseTable::SampleDatabaseTable(TimeSliceThread& inThread)
{
    mDirectoryList = std::make_unique<DirectoryContentsList>(nullptr, inThread);
    mDirectoryList->setDirectory(File::getSpecialLocation(File::userMusicDirectory), true, true);
    
    inThread.startThread(Thread::Priority::normal);
}

SampleDatabaseTable::~SampleDatabaseTable()
{
    
}

DirectoryContentsList* SampleDatabaseTable::getDirectoryList()
{
    return &*mDirectoryList;
}
