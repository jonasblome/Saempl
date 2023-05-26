/*
  ==============================================================================

    SampleItemViewModel.h
    Created: 22 May 2023 6:31:17am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleItem.h"

class SampleItemViewModel
{
public:
    SampleItemViewModel();
    ~SampleItemViewModel();
    
private:
    SampleItem loadedSample;
};
