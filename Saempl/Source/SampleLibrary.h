/*
  ==============================================================================

    SampleLibrary.h
    Created: 21 May 2023 2:45:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once
#include "SampleItem.h"
#include "BlomeHelperFunctions.h"
#include "SampleAnalyser.h"
#include "SampleLibraryManager.h"

class SampleLibrary
:   ChangeListener
{
public:
    // Constructors
    SampleLibrary(TimeSliceThread& inThread);
    ~SampleLibrary();
    
    // Methods
    void addSampleItem(const File& inFile);
    void removeSampleItem(const String& inFilePath, bool deletePermanently);
    DirectoryContentsList* getDirectoryList();
    void changeListenerCallback(ChangeBroadcaster* inSource) override;
    void refresh();
    SampleItem* getSampleItemWithFilePath(const String& inFilePath);
    void setDirectory(String inDirectoryPath);
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleLibrary)
    
    // Fields
    std::unique_ptr<FileFilter> mDirectoryFilter;
    String mCurrentLibraryPath;
    std::unique_ptr<DirectoryContentsList> mDirectoryContent;
    OwnedArray<SampleItem> mSampleItems;
    String mDirectoryPathToAddFilesTo;
    std::unique_ptr<SampleAnalyser> mSampleAnalyser;
    std::unique_ptr<SampleLibraryManager> mSampleLibraryManager;
    
    // Methods
    void createSampleItem(File inFile);
};
