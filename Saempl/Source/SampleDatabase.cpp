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
    // Set directory path
    mSampleItemDirectoryPath =
    (File::getSpecialLocation(juce::File::userDesktopDirectory)).getFullPathName() + DIRECTORY_SEPARATOR + "SampleItemDirectory";
    
    if(!File(mSampleItemDirectoryPath).exists())
    {
        File(mSampleItemDirectoryPath).createDirectory();
    }
    
    // Set directory with path and file filter
    mDirectoryList = std::make_unique<DirectoryContentsList>(&mDirectoryFilter, inThread);
    mDirectoryList->addChangeListener(this);
    mDirectoryList->setDirectory(File(mSampleItemDirectoryPath), true, true);
    
    mSampleAnalyser = std::make_unique<SampleAnalyser>();
}

SampleDatabase::~SampleDatabase()
{
    mDirectoryList->removeChangeListener(this);
}

void SampleDatabase::addSampleItem(File inFile)
{
    String fileName = inFile.getFileName();
    File newFile = File(mDirectoryPathToAddFilesTo + DIRECTORY_SEPARATOR + fileName);
    
    // Don't add files if they already exist
    if (mDirectoryList->contains(newFile)) {
        return;
    }
    
    if (inFile.isDirectory())
    {
        // Check directory recursively for audio files and subdirectoryies
        mDirectoryPathToAddFilesTo = newFile.getFullPathName();
        bool newDirectoryWasCreated = newFile.createDirectory();
        
        if(newDirectoryWasCreated)
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
        // Add file to current directory and create SampleItem
        inFile.copyFileTo(newFile);
        mSampleItems.add(new SampleItem());
        mSampleItems.getLast()->setFilePath(newFile.getFullPathName());
        mSampleItems.getLast()->addSampleTag(new SampleTag("Length", mSampleAnalyser->analyseSampleLength(newFile)));
    }
    
    mDirectoryList->refresh();
}

void SampleDatabase::loadSampleItem(SampleItem inItem)
{
    
}

void SampleDatabase::removeSampleItem(String inFilePath, bool deletePermanently = false)
{
    SampleItem* itemToDelete = getSampleItemWithFilePath(inFilePath);
    mSampleItems.removeObject(itemToDelete);
    
    if (deletePermanently) {
        File(inFilePath).deleteRecursively();
    }
    else
    {
        File(inFilePath).moveToTrash();
    }
    
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

void SampleDatabase::switchToParentDirectory()
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

SampleItem* SampleDatabase::getSampleItemWithFilePath(String inFilePath)
{
    for(SampleItem* item : mSampleItems)
    {
        if (item->getFilePath() == inFilePath) {
            return item;
        }
    }
    
    return nullptr;
}
