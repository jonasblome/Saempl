/*
  ==============================================================================

    SampleManagerThread.cpp
    Created: 13 Feb 2024 1:07:43pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleManagerThread.h"

SampleManagerThread::SampleManagerThread(OwnedArray<SampleItem>& inSampleItems, OwnedArray<SampleItem>& inAddedSampleItems, StringArray& inAddedFilePaths, File const & inFile, SampleItem* inSampleItem, bool forceAnalysis, int& inNumProcessedItems)
:
ThreadPoolJob("SampleManagerThread"),
allSampleItems(inSampleItems),
addedSampleItems(inAddedSampleItems),
addedFilePaths(inAddedFilePaths),
file(inFile),
sampleItem(inSampleItem),
mForceAnalysis(forceAnalysis),
numProcessedItems(inNumProcessedItems)
{
    // Initialize sample analyser
    mSampleAnalyser = std::make_unique<SampleAnalyser>();
}

SampleManagerThread::~SampleManagerThread()
{
    
}

ThreadPoolJob::JobStatus SampleManagerThread::runJob()
{
    if (sampleItem == nullptr)
    {
        sampleItem = allSampleItems.add(new SampleItem());
        sampleItem->setFilePath(file.getFullPathName().convertToPrecomposedUnicode());
        sampleItem->setTitle(file.getFileNameWithoutExtension().convertToPrecomposedUnicode());
        addedFilePaths.add(sampleItem->getFilePath());
        addedSampleItems.add(sampleItem);
        numProcessedItems++;
    }
    
    mSampleAnalyser->analyseSample(sampleItem, mForceAnalysis);
    
    return jobHasFinished;
}
