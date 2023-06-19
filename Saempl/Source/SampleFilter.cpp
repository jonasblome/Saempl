/*
  ==============================================================================

    SampleItemFilter.cpp
    Created: 10 Jun 2023 6:14:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

# include "SampleFilter.h"

SampleFilter::SampleFilter(String inDescription)
:   FileFilter(inDescription)
{
    
}

SampleFilter::~SampleFilter()
{
    
}

/** Should return true if this file is suitable for inclusion in whatever context
    the object is being used.
*/
bool SampleFilter::isFileSuitable(const File& inFile) const
{
    auto* e = mFilteredFilePaths.begin();
    
    for (; e != mFilteredFilePaths.end(); ++e)
    {
        if (**e == inFile.getFullPathName())
        {
            return true;
        }
    }
    
    return false;
}

/** Should return true if this directory is suitable for inclusion in whatever context
    the object is being used.
*/
bool SampleFilter::isDirectorySuitable(const File& inDirectory) const
{
    for (DirectoryEntry entry : RangedDirectoryIterator(inDirectory, true, "*.bsf", File::findFiles))
    {
        if (isFileSuitable(entry.getFile()))
        {
            return true;
        }
    }
    
    return false;
}

void SampleFilter::addFilteredFilePath(String inFilePath)
{
    mFilteredFilePaths.add(new String(inFilePath));
}

void SampleFilter::clear()
{
    mFilteredFilePaths.clear();
}
