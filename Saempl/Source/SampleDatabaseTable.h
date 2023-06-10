/*
  ==============================================================================

    SampleDatabaseTable.h
    Created: 21 May 2023 3:09:08pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleFileFilter.h"

class SampleDatabaseTable
{
public:
    // Constructors
    SampleDatabaseTable(TimeSliceThread& inThread);
    ~SampleDatabaseTable();
    
    // Methods
    
private:
    // Fields
    SampleFileFilter mSampleFileFilter;
    
    // Methods
    
};
