/*
  ==============================================================================

    SampleLibrary.cpp
    Created: 21 May 2023 2:45:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibrary.h"

SampleLibrary::SampleLibrary(TimeSliceThread& inThread)
{
    // Initialize library manager
    mSampleLibraryManager = std::make_unique<SampleLibraryManager>();
    
    // Set file filter
    mFileFilter = std::make_unique<SampleFileFilter>("AudioFormatsFilter", mFilteredSampleItems);
    mDirectoryContent = std::make_unique<DirectoryContentsList>(&*mFileFilter, inThread);
    
    // Set directory
    mDirectoryContent->addChangeListener(this);
    String lastOpenedDirectory = mSampleLibraryManager->getLastOpenedDirectory();
    setDirectory(lastOpenedDirectory);
}

SampleLibrary::~SampleLibrary()
{
    mSampleLibraryManager->storeLastOpenedDirectory(mCurrentLibraryPath);
    mDirectoryContent->removeChangeListener(this);
    File currentLibrary(mCurrentLibraryPath);
    mSampleLibraryManager->updateSampleLibraryFile(currentLibrary, mAllSampleItems);
    clearSampleItemCollections();
}

void SampleLibrary::addToSampleItems(File const & inFile)
{
    String fileName = inFile.getFileName();
    File newFile = File(mDirectoryPathToAddFilesTo + DIRECTORY_SEPARATOR + fileName);
    
    // Don't add files if they already exist in the current library
    if (mSampleLibraryManager->getSampleItemWithFileName(newFile.getFileName(), mAllSampleItems) != nullptr)
    {
        return;
    }
    
    // Check directory recursively for audio files and subdirectories
    if (inFile.isDirectory())
    {
        mDirectoryPathToAddFilesTo = newFile.getFullPathName();
        bool newDirectoryWasCreated = newFile.createDirectory();
        
        if (newDirectoryWasCreated)
        {
            for (DirectoryEntry entry : RangedDirectoryIterator(inFile, false, "*", File::findFilesAndDirectories))
            {
                addToSampleItems(entry.getFile());
            }
        }
        
        mDirectoryPathToAddFilesTo = newFile.getParentDirectory().getFullPathName();
    }
    else if (isSupportedAudioFileFormat(newFile.getFileExtension()))
    {
        // Add file to current directory and create SampleItem
        inFile.copyFileTo(newFile);
        mSampleLibraryManager->createSampleItem(newFile, mAllSampleItems);
    }
}

void SampleLibrary::addToPalette(File const & inFile)
{
    addToSampleItems(inFile);
    SampleItem* itemToAdd = mSampleLibraryManager->getSampleItemWithFileName(inFile.getFileName(), mAllSampleItems);
    
    // Check if item is already in palette
    if (!mPaletteSampleItems.contains(itemToAdd))
    {
        mPaletteSampleItems.add(itemToAdd);
    }
}

void SampleLibrary::removeSampleItem(String const & inFilePath, bool deletePermanently = false)
{
    // Delete sample item
    SampleItem* itemToDelete = mSampleLibraryManager->getSampleItemWithFileName(File(inFilePath).getFileName(),
                                                                                mAllSampleItems);
    mAllSampleItems.removeObject(itemToDelete);
    removeFromPalette(*itemToDelete);
    
    // Delete audio file
    File fileToDelete = File(inFilePath);
    
    if (fileToDelete.exists())
    {
        deletePermanently ? fileToDelete.deleteRecursively() : fileToDelete.moveToTrash();
    }
}

void SampleLibrary::removeFromPalette(SampleItem& inSampleItem)
{
    mPaletteSampleItems.removeObject(&inSampleItem, false);
}

DirectoryContentsList& SampleLibrary::getDirectoryList()
{
    return *mDirectoryContent;
}

SampleFileFilter& SampleLibrary::getFileFilter()
{
    return *mFileFilter;
}

void SampleLibrary::changeListenerCallback(ChangeBroadcaster* inSource)
{
    // Set new directory if directory changes
    if (inSource == mDirectoryContent.get())
    {
        mDirectoryPathToAddFilesTo = mDirectoryContent->getDirectory().getFullPathName();
    }
}

/**
 Updates all sample files in the current directory and updates the corresponding SampleItem collection
 */
void SampleLibrary::refresh()
{
    // Go through all current sample items, check if corresponding audio file still exists
    // and if not, delete sample item from all items and palette collection
    for (SampleItem* sampleItem : mAllSampleItems)
    {
        if (!File(sampleItem->getFilePath()).exists())
        {
            mPaletteSampleItems.removeObject(sampleItem, false);
            mAllSampleItems.removeObject(sampleItem);
        }
    }
    
    applyFilter();
    mDirectoryContent->refresh();
    sendChangeMessage();
}

void SampleLibrary::setDirectory(String inDirectoryPath)
{
    File currentLibraryDirectory(mCurrentLibraryPath);
    
    // If a library is currently loaded, store it
    if (mCurrentLibraryPath != "")
    {
        mSampleLibraryManager->updateSampleLibraryFile(currentLibraryDirectory, mAllSampleItems);
    }
    
    mCurrentLibraryPath = inDirectoryPath;
    currentLibraryDirectory = File(mCurrentLibraryPath);
    
    if (!currentLibraryDirectory.exists())
    {
        currentLibraryDirectory.createDirectory();
    }
    
    // Load new library
    clearSampleItemCollections();
    mSampleLibraryManager->loadSampleLibrary(currentLibraryDirectory, mAllSampleItems);
    mDirectoryContent->setDirectory(File(mCurrentLibraryPath), true, true);
    refresh();
}

OwnedArray<SampleItem>& SampleLibrary::getSampleItems(SampleItemCollectionScope inCollectionScope)
{
    switch (inCollectionScope)
    {
        case ALL_SAMPLES:
            return mAllSampleItems;
            break;
        case FILTERED_SAMPLES:
            return mFilteredSampleItems;
            break;
        case PALETTE_SAMPLES:
            return mPaletteSampleItems;
            break;
        default:
            jassertfalse;
            break;
    }
}

void SampleLibrary::applyFilter()
{
    mFilteredSampleItems.clear(false);
    
    for (SampleItem* sampleItem : mAllSampleItems)
    {
        if (mFileFilter->matchesRules(*sampleItem))
        {
            mFilteredSampleItems.add(sampleItem);
        }
    }
}

String SampleLibrary::getCurrentLibraryPath()
{
    return mCurrentLibraryPath;
}

void SampleLibrary::clearSampleItemCollections()
{
    mPaletteSampleItems.clear(false);
    mFilteredSampleItems.clear(false);
    mAllSampleItems.clear();
}

double& SampleLibrary::getLoadingProgress()
{
    return loadingProgress;
}
