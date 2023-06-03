/*
  ==============================================================================

    SampleDatabase.cpp
    Created: 21 May 2023 2:45:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleDatabase.h"

#if JUCE_WINDOWS
    static const juce::String directorySeparator = "\\";
#elif JUCE_MAC
    static const juce::String directorySeparator = "/";
#endif

SampleDatabase::SampleDatabase(TimeSliceThread& inThread)
{
    mSampleItemDirectory =
    (File::getSpecialLocation(juce::File::userDesktopDirectory)).getFullPathName() + directorySeparator + "SampleItemDirectory";
    
    if(!File(mSampleItemDirectory).exists()) {
        File(mSampleItemDirectory).createDirectory();
    }
    
    mDirectoryList = std::make_unique<DirectoryContentsList>(nullptr, inThread);
    mDirectoryList->setDirectory(File(mSampleItemDirectory), true, true);
}

SampleDatabase::~SampleDatabase()
{
    
}

void SampleDatabase::addSampleItem(String inFilePath)
{
    File oldFile = File(inFilePath);
    String fileName = oldFile.getFileName();
    File newFile = File(mSampleItemDirectory + directorySeparator + fileName);
    oldFile.copyFileTo(newFile);
    mDirectoryList->refresh();
}

void SampleDatabase::addSampleItem(SampleItem inItem)
{
    
}

void SampleDatabase::removeSampleItem(String inFilePath)
{
    File(inFilePath).deleteFile();
    mDirectoryList->refresh();
}

DirectoryContentsList* SampleDatabase::getDirectoryList()
{
    return &*mDirectoryList;
}
