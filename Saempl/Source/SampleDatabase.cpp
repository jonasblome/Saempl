/*
  ==============================================================================

    SampleDatabase.cpp
    Created: 21 May 2023 2:45:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleDatabase.h"

SampleDatabase::SampleDatabase(TimeSliceThread& inThread)
:   mDirectoryFilter("*.wav;*.mp3;.m4a", "*", "audio files")
{
    mSampleItemDirectoryPath =
    (File::getSpecialLocation(juce::File::userDesktopDirectory)).getFullPathName() + DIRECTORY_SEPARATOR + "SampleItemDirectory";
    
    if(!File(mSampleItemDirectoryPath).exists())
    {
        File(mSampleItemDirectoryPath).createDirectory();
    }
    
    mDirectoryList = std::make_unique<DirectoryContentsList>(&mDirectoryFilter, inThread);
    mDirectoryList->addChangeListener(this);
    mDirectoryList->setDirectory(File(mSampleItemDirectoryPath), true, true);
}

SampleDatabase::~SampleDatabase()
{
    mDirectoryList->removeChangeListener(this);
}

void SampleDatabase::addSampleItem(File inFile)
{
    bool isDirectory = inFile.isDirectory();
    
    String fileName = inFile.getFileName();
    File newFile = File(mDirectoryPathToAddFilesTo + DIRECTORY_SEPARATOR + fileName);
    
    if (isDirectory)
    {
        mDirectoryPathToAddFilesTo = newFile.getFullPathName();
        bool directoryCreated = newFile.createDirectory();
        
        if(directoryCreated)
        {
            for (DirectoryEntry entry : RangedDirectoryIterator(inFile, false, "*", File::findFilesAndDirectories))
            {
                addSampleItem(entry.getFile());
            }
        }
        
        mDirectoryPathToAddFilesTo = newFile.getParentDirectory().getFullPathName();
    }
    else if(isSupportedAudioFileFormat(newFile.getFileExtension()))
    {
        inFile.copyFileTo(newFile);
    }
    
    mDirectoryList->refresh();
}

void SampleDatabase::addSampleItem(SampleItem inItem)
{
    
}

void SampleDatabase::removeSampleItem(String inFilePath)
{
    File(inFilePath).deleteRecursively();
    mDirectoryList->refresh();
}

void SampleDatabase::moveSampleItemToTrash(String inFilePath)
{
    File(inFilePath).moveToTrash();
    mDirectoryList->refresh();
}

DirectoryContentsList* SampleDatabase::getDirectoryList()
{
    return &*mDirectoryList;
}

void SampleDatabase::setDirectory(const File& inFile)
{
    mDirectoryList->setDirectory(inFile, true, true);
}

void SampleDatabase::setToParentDirectory()
{
    File parentDirectory = mDirectoryList->getDirectory().getParentDirectory();
    setDirectory(parentDirectory);
}

void SampleDatabase::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == mDirectoryList.get())
    {
        mDirectoryPathToAddFilesTo = mDirectoryList->getDirectory().getFullPathName();
    }
}
