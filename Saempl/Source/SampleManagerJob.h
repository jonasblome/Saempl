/*
 ==============================================================================
 
 SampleManagerJob.h
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
    /**
     The sample manager job constructor.
     
     @param inSampleItems a reference to the sample items collection.
     @param inAddedSampleItems a reference to the added sample items collection.
     @param inAddedFilePath a reference to the array of currently added file paths.
     @param inFile the file to the sample to analyse.
     @param inSampleItem a pointer to the sample item to analyse.
     @param forceAnalysis see sample analyser analysis method.
     @param numProcessedItems a reference to the item counter.
     */
    SampleManagerThread(OwnedArray<SampleItem>& inSampleItems,
                        OwnedArray<SampleItem>& inAddedSampleItems,
                        StringArray& inAddedFilePaths,
                        File const & inFile,
                        SampleItem* inSampleItem,
                        bool forceAnalysis,
                        int& numProcessedItems);
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
     Runs the analysis of a sample item and creates a new one if the pointer is null.
     */
    ThreadPoolJob::JobStatus runJob() override;
};
