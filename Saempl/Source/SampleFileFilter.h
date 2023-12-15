/*
 ==============================================================================
 
 SampleFileFilter.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"
#include "BlomeHelpers.h"
#include "SampleFileFilterRuleBase.h"

/**
 The filter class to check if \ref SampleItem objects and files are matching the filter rules.
 
 Implements:
 \ref FileFilter
 Holds a collection of \ref SampleFileFilterRuleBase objects.
 */
class SampleFileFilter
:
public FileFilter
{
public:
    /**
     The constructor for the sample file filter.
     
     @param inDescription the description of the file filter.
     @param inFilteredSampleItems the collection of filtered sample items in the library.
     */
    SampleFileFilter(String const & inDescription, OwnedArray<SampleItem>& inFilteredSampleItems);
    ~SampleFileFilter();
    /**
     Checks if all filter rules apply to the sample item.
     
     @param inSampleItem the sample to apply the filter to.
     
     @returns whether the sample item matches all the rules.
     */
    bool matchesRules(SampleItem& inSampleItem);
    /**
     Adds a filter rule to the rule collection.
     
     @returns the added rule.
     */
    SampleFileFilterRuleBase* addFilterRule(SampleFileFilterRuleBase* inRule);
    /**
     @returns the collection of filter rules.
     */
    OwnedArray<SampleFileFilterRuleBase>& getFilterRules();
    /**
     @returns whether the filter is toggled on or off.
     */
    bool getIsActive();
    /**
     Sets the filter to on or off.
     
     @param inIsActive true for to active, false for to inactive.
     */
    void setIsActive(bool inIsActive);
    
private:
    OwnedArray<SampleFileFilterRuleBase> mFilterRules;
    OwnedArray<SampleItem>& filteredSampleItems;
    bool isActive;
    
    bool isFileSuitable (const File& file) const override;
    bool isDirectorySuitable (const File& file) const override;
};
