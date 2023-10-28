/*
  ==============================================================================

    SampleFileFilter.h
    Created: 10 Jun 2023 6:14:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"
#include "BlomeHelpers.h"
#include "SampleFileFilterRule.h"

class SampleFileFilter
:   public FileFilter
{
public:
    // Constructors
    SampleFileFilter(String const & inDescription, OwnedArray<SampleItem>& inFilteredSampleItems);
    ~SampleFileFilter();
    
    // Methods
    bool isFileSuitable (const File& file) const override;
    bool isDirectorySuitable (const File& file) const override;
    bool matchesRules(SampleItem& inSampleItem);
    void addFilterRule(SampleFileFilterRule* inRule);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleFileFilter)
    
    // Fields
    OwnedArray<SampleFileFilterRule> mFilterRules;
    OwnedArray<SampleItem>& filteredSampleItems;
    
    // Methods
    
};
