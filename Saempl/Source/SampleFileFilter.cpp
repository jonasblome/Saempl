/*
 ==============================================================================
 
 SampleFileFilter.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFileFilter.h"

SampleFileFilter::SampleFileFilter(String const & inDesciption, OwnedArray<SampleItem>& inFilteredSampleItems)
:
FileFilter(inDesciption),
filteredSampleItems(inFilteredSampleItems)
{
    isActive = true;
}

SampleFileFilter::~SampleFileFilter()
{
    
}

bool SampleFileFilter::isFileSuitable(File const & file) const
{
    for (SampleItem * sampleItem: filteredSampleItems)
    {
        if (sampleItem == nullptr)
        {
            return false;
        }
        
        if (sampleItem->getFilePath() == file.getFullPathName())
        {
            return true;
        }
    }
    
    return false;
}

bool SampleFileFilter::isDirectorySuitable(File const & file) const
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
        if (rule->canHaveEffect() && !rule->matches(inSampleItem))
        {
            return false;
        }
    }
    
    return true;
}

SampleFileFilterRuleBase* SampleFileFilter::addFilterRule(SampleFileFilterRuleBase* inFilterRule)
{
    mFilterRules.add(inFilterRule);
    return inFilterRule;
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

bool SampleFileFilter::canHaveEffect()
{
    if (!isActive)
    {
        return false;
    }
    
    for (SampleFileFilterRuleBase* rule : mFilterRules)
    {
        if (rule->canHaveEffect())
        {
            return true;
        }
    }
    
    return false;
}
