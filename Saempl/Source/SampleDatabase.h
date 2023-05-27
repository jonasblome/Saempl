/*
  ==============================================================================

    SampleDatabase.h
    Created: 21 May 2023 2:45:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once
#include "SampleItem.h"

class SampleDatabase
{
public:
    SampleDatabase();
    ~SampleDatabase();
    
    void addSampleItem(SampleItem inItem);
    void removeSampleItem(String itemID);
    
private:
    OwnedArray<SampleItem> mSampleItems;
};
