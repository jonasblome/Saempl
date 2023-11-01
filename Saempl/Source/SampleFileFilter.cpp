/*
  ==============================================================================

    SampleFileFilter.cpp
    Created: 10 Jun 2023 6:14:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileFilter.h"

SampleFileFilter::SampleFileFilter(String const & inDesciption, OwnedArray<SampleItem>& inFilteredSampleItems)
:   FileFilter(inDesciption),
    filteredSampleItems(inFilteredSampleItems)
{
    isActive = true;
}

SampleFileFilter::~SampleFileFilter()
{
    
}

bool SampleFileFilter::isFileSuitable (const File& file) const
{
    for (SampleItem* sampleItem: filteredSampleItems)
    {
        if (sampleItem->getFilePath() == file.getFullPathName())
        {
            return true;
        }
    }
    
    return false;
}

bool SampleFileFilter::isDirectorySuitable (const File& file) const
{
    return !file.findChildFiles(File::findFiles, true, SUPPORTED_AUDIO_FORMATS_WILDCARD).isEmpty();
}

bool SampleFileFilter::matchesRules(SampleItem& inSampleItem)
{
    if (!isActive)
    {
        return true;
    }
    
    // Check if all active rules apply
    for (SampleFileFilterRuleBase* rule : mFilterRules)
    {
        if (rule->getIsActive() && !rule->matches(inSampleItem))
        {
            return false;
        }
    }
    
    return true;
}

SampleFileFilterRuleBase* SampleFileFilter::addFilterRule(SampleFileFilterRuleBase* inFilterRule)
{
    return mFilterRules.add(inFilterRule);
}

void SampleFileFilter::setFilteredSampleItems(OwnedArray<SampleItem>& inFilteredSampleItems)
{
    filteredSampleItems.swapWith(inFilteredSampleItems);
}

OwnedArray<SampleFileFilterRuleBase>& SampleFileFilter::getFilterRules()
{
    return mFilterRules;
}

bool SampleFileFilter::getIsActive()
{
    return isActive;
}

void SampleFileFilter::setIsActive(bool inIsActive)
{
    isActive = inIsActive;
}
