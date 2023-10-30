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
#include "SampleFileFilterRuleBase.h"

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
    SampleFileFilterRuleBase* addFilterRule(SampleFileFilterRuleBase* inRule);
    void setFilteredSampleItems(OwnedArray<SampleItem>& inFilteredSampleItems);
    OwnedArray<SampleFileFilterRuleBase>& getFilterRules();
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleFileFilter)
    
    // Fields
    OwnedArray<SampleFileFilterRuleBase> mFilterRules;
    OwnedArray<SampleItem>& filteredSampleItems;
    
    // Methods
    
};
