/*
  ==============================================================================

    SampleDatabaseTableViewModel.h
    Created: 21 May 2023 3:14:48pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "SampleDatabaseTable.h"

class SampleDatabaseTableViewModel
{
public:
    SampleDatabaseTableViewModel(TimeSliceThread& inThread);
    ~SampleDatabaseTableViewModel();
    
    DirectoryContentsList* getDirectoryList();
    
private:
    SampleDatabaseTable mSampleDatabaseTable;
    
};
