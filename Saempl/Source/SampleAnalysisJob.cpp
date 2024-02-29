/*
 ==============================================================================
 
 SampleAnalysisJob.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleAnalysisJob.h"

SampleAnalysisJob::SampleAnalysisJob(OwnedArray<SampleItem>& inSampleItems,
                                         OwnedArray<SampleItem>& inAddedSampleItems,
                                         StringArray& inAddedFilePaths,
                                         File const & inFile,
                                         SampleItem* inSampleItem,
                                         bool forceAnalysis,
                                         int& inNumProcessedItems)
:
ThreadPoolJob("SampleAnalysisJob"),
allSampleItems(inSampleItems),
addedSampleItems(inAddedSampleItems),
addedFilePaths(inAddedFilePaths),
file(inFile),
sampleItem(inSampleItem),
mForceAnalysis(forceAnalysis),
numProcessedItems(inNumProcessedItems)
{
    // Initialise sample analyser
    mSampleAnalyser = std::make_unique<SampleAnalyser>();
}

SampleAnalysisJob::~SampleAnalysisJob()
{
    
}

ThreadPoolJob::JobStatus SampleAnalysisJob::runJob()
{
    if (sampleItem == nullptr)
    {
        sampleItem = allSampleItems.add(new SampleItem());
        String filePath = file.getFullPathName();
        String sampleTitle = file.getFileNameWithoutExtension();
#if JUCE_MAC
        filePath = filePath.convertToPrecomposedUnicode();
        sampleTitle = sampleTitle.convertToPrecomposedUnicode();
#endif
        sampleItem->setFilePath(filePath);
        sampleItem->setTitle(sampleTitle);
        addedFilePaths.add(sampleItem->getFilePath());
        addedSampleItems.add(sampleItem);
        mSampleAnalyser->analyseSample(sampleItem, mForceAnalysis);
        numProcessedItems++;
        return jobHasFinished;
    }
    else
    {
        mSampleAnalyser->analyseSample(sampleItem, mForceAnalysis);
        return jobHasFinished;
    }
}
