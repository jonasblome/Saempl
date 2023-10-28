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
    for (SampleFileFilterRuleBase* rule : mFilterRules)
    {
        if (!rule->matches(inSampleItem))
        {
            return false;
        }
    }
    
    return true;
}

void SampleFileFilter::addFilterRule(SampleFileFilterRuleBase* inFilterRule)
{
    mFilterRules.add(inFilterRule);
}
