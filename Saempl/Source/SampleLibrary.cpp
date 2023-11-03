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
    // Initialize sample analyser
    mSampleAnalyser = std::make_unique<SampleAnalyser>();
    
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
    if (getSampleItemWithFileName(newFile.getFileName()) != nullptr)
    {
        return;
    }
    
    if (inFile.isDirectory())
    {
        // Check directory recursively for audio files and subdirectoryies
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
        createSampleItem(newFile);
    }
    else
    {
        return;
    }
}

void SampleLibrary::addToPalette(File const & inFile)
{
    addToSampleItems(inFile);
    SampleItem* itemToAdd = getSampleItemWithFileName(inFile.getFileName());
    
    // Check if item is already in palette
    if (!mPaletteSampleItems.contains(itemToAdd))
    {
        mPaletteSampleItems.add(itemToAdd);
    }
}

void SampleLibrary::removeSampleItem(String const & inFilePath, bool deletePermanently = false)
{
    // Delete sample item
    SampleItem* itemToDelete = getSampleItemWithFileName(File(inFilePath).getFileName());
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
    
    // Go through all files in directory, check if a corresponding sample item
    // already exists in the sample item list, if not add it
    for (DirectoryEntry entry : RangedDirectoryIterator(mDirectoryContent->getDirectory(),
                                                        true,
                                                        SUPPORTED_AUDIO_FORMATS_WILDCARD,
                                                        File::findFiles))
    {
        bool linkedSampleItemExists = getSampleItemWithFileName(entry.getFile().getFileName()) != nullptr;
        
        if (not linkedSampleItemExists)
        {
            createSampleItem(entry.getFile());
        }
    }
    
    applyFilter();
    mDirectoryContent->refresh();
}

SampleItem* SampleLibrary::getSampleItemWithFileName(String const & inFileName)
{
    for (SampleItem* sampleItem : mAllSampleItems)
    {
        if (File(sampleItem->getFilePath()).getFileName().compare(inFileName) == 0)
        {
            return sampleItem;
        }
    }
    
    return nullptr;
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
    mSampleLibraryManager->loadSampleLibraryFile(currentLibraryDirectory, mAllSampleItems);
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

/**
 Creates a SampleItem for a file and adds it to the collection.
 If the corresponding SampleItem already exists, nothing happens.
 */
void SampleLibrary::createSampleItem(File inFile)
{
    // Check if item already exists
    bool linkedSampleItemExists = getSampleItemWithFileName(inFile.getFileName()) != nullptr;
    if (!linkedSampleItemExists)
    {
        SampleItem* newItem = mAllSampleItems.add(new SampleItem());
        newItem->setFilePath(inFile.getFullPathName());
        newItem->setTitle(inFile.getFileNameWithoutExtension());
        newItem->setLength(mSampleAnalyser->analyseSampleLength(inFile));
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
