/*
  ==============================================================================

    SampleManagerThread.h
    Created: 13 Feb 2024 1:07:43pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleAnalyser.h"

class SampleManagerThread
:
public ThreadPoolJob
{
public:
    SampleManagerThread(OwnedArray<SampleItem>& inSampleItems, OwnedArray<SampleItem>& inAddedSampleItems, StringArray& inAddedFilePaths, File const & inFile, SampleItem* inSampleItem, bool forceAnalysis, int& numProcessedItems);
    ~SampleManagerThread();
    
private:
    std::unique_ptr<SampleAnalyser> mSampleAnalyser;
    OwnedArray<SampleItem>& allSampleItems;
    OwnedArray<SampleItem>& addedSampleItems;
    StringArray& addedFilePaths;
    File const & file;
    SampleItem* sampleItem;
    bool mForceAnalysis;
    int& numProcessedItems;
    
    /**
     Runs the analysis of a sample item.
     */
    ThreadPoolJob::JobStatus runJob() override;
};
