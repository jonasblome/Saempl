/*
  ==============================================================================

    SampleFileFilter.cpp
    Created: 10 Jun 2023 6:14:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileFilter.h"

SampleFileFilter::SampleFileFilter(String& inDescription)
:   FileFilter(inDescription)
{
    
}

SampleFileFilter::~SampleFileFilter()
{
    
}

/** Should return true if this file is suitable for inclusion in whatever context
    the object is being used.
*/
bool SampleFileFilter::isFileSuitable(const File& file) const
{
    return false;
}

/** Should return true if this directory is suitable for inclusion in whatever context
    the object is being used.
*/
bool SampleFileFilter::isDirectorySuitable(const File& file) const
{
    return false;
}

void SampleFileFilter::addFilteredFilePath(SampleItem* inSampleItem)
{
    mFilteredFilePaths.add(inSampleItem);
}
