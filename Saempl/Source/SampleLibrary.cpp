/*
  ==============================================================================

    SampleLibrary.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibrary.h"

SampleLibrary::SampleLibrary()
{
    // Create thread for scanning the sample library directory
    mDirectoryScannerThread = std::make_unique<TimeSliceThread>("DirectoryReaderThread");
    mDirectoryScannerThread->startThread(Thread::Priority::normal);
    
    // Initialize library manager
    mSampleLibraryManager = std::make_unique<SampleLibraryManager>(mAllSampleItems, mPaletteSampleItems);
    mSampleLibraryManager->addChangeListener(this);
    
    // Set file filter
    mFileFilter = std::make_unique<SampleFileFilter>("AudioFormatsFilter", mFilteredSampleItems);
    mDirectoryContentsList = std::make_unique<DirectoryContentsList>(&*mFileFilter, *mDirectoryScannerThread);
    
    // Set directory
    mDirectoryContentsList->addChangeListener(this);
    String lastOpenedDirectory = mSampleLibraryManager->getLastOpenedDirectory();
    setDirectory(lastOpenedDirectory);
}

SampleLibrary::~SampleLibrary()
{
    mSampleLibraryManager->storeLastOpenedDirectory(mCurrentLibraryPath);
    mSampleLibraryManager->removeChangeListener(this);
    mDirectoryContentsList->removeChangeListener(this);
    File currentLibrary(mCurrentLibraryPath);
    mSampleLibraryManager->updateSampleLibraryFile(currentLibrary);
    clearSampleItemCollections();
    mDirectoryScannerThread->stopThread(10000);
}

SampleItem* SampleLibrary::addToSampleItems(File const & inFile)
{
    // Add file to current directory and create SampleItem
    String fileName = inFile.getFileName();
    File newFile = File(mDirectoryPathToAddFilesTo + DIRECTORY_SEPARATOR + fileName);
    inFile.copyFileTo(newFile);
    return mSampleLibraryManager->createSampleItem(newFile);
}

void SampleLibrary::addAllToSampleItems(File const & inFile)
{
    String fileName = inFile.getFileName();
    File newFile = File(mDirectoryPathToAddFilesTo + DIRECTORY_SEPARATOR + fileName);
    
    // Don't add files if they already exist in the current library
    if (mSampleLibraryManager->fileHasBeenAdded(inFile.getFullPathName()))
    {
        return;
    }
    else if (mSampleLibraryManager->fileHasBeenAdded(newFile.getFullPathName()))
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
                addAllToSampleItems(entry.getFile());
            }
        }
        
        mDirectoryPathToAddFilesTo = newFile.getParentDirectory().getFullPathName();
    }
    else if (isSupportedAudioFileFormat(newFile.getFileExtension()))
    {
        addToSampleItems(inFile);
        refresh();
    }
}

void SampleLibrary::addToPalette(const File & inFile)
{
    SampleItem* itemToAdd = mSampleLibraryManager->getSampleItemWithFilePath(inFile.getFullPathName());
    
    // Create item if it doesn't yet exist
    if (itemToAdd == nullptr)
    {
        itemToAdd = addToSampleItems(inFile);
        refresh();
    }
    
    // Check if item is already in palette
    if (!mPaletteSampleItems.contains(itemToAdd))
    {
        mPaletteSampleItems.add(itemToAdd);
    }
}

void SampleLibrary::addAllToPalette(File const & inFile)
{
    if (inFile.isDirectory())
    {
        Array<File> sampleFiles = inFile.findChildFiles(File::findFiles, true, SUPPORTED_AUDIO_FORMATS_WILDCARD);
        
        for (File sampleFile : sampleFiles)
        {
            addToPalette(sampleFile);
        }
    }
    else if (isSupportedAudioFileFormat(inFile.getFileExtension()))
    {
        addToPalette(inFile);
    }
}

void SampleLibrary::removeSampleItem(String const & inFilePath, bool deletePermanently = false)
{
    File fileToDelete = File(inFilePath);
    
    // Delete sample item
    SampleItem* itemToDelete = mSampleLibraryManager->getSampleItemWithFilePath(fileToDelete.getFullPathName());
    removeFromPalette(*itemToDelete);
    mAllSampleItems.removeObject(itemToDelete);
    
    // Delete audio file
    if (fileToDelete.exists())
    {
        deletePermanently ? fileToDelete.deleteRecursively() : fileToDelete.moveToTrash();
    }
    
    refresh();
}

void SampleLibrary::removeFromPalette(SampleItem& inSampleItem)
{
    mPaletteSampleItems.removeObject(&inSampleItem, false);
}

DirectoryContentsList& SampleLibrary::getDirectoryList()
{
    return *mDirectoryContentsList;
}

SampleFileFilter& SampleLibrary::getFileFilter()
{
    return *mFileFilter;
}

void SampleLibrary::refresh()
{
    applyFilter();
    mDirectoryContentsList->refresh();
}

void SampleLibrary::setDirectory(String inDirectoryPath)
{
    File currentLibraryDirectory(mCurrentLibraryPath);
    
    // If a library is currently loaded, store it
    if (mCurrentLibraryPath != "")
    {
        mSampleLibraryManager->updateSampleLibraryFile(currentLibraryDirectory);
    }
    
    mCurrentLibraryPath = inDirectoryPath;
    currentLibraryDirectory = File(mCurrentLibraryPath);
    
    if (!currentLibraryDirectory.exists())
    {
        currentLibraryDirectory.createDirectory();
    }
    
    // Load new library
    clearSampleItemCollections();
    mSampleLibraryManager->loadSampleLibrary(currentLibraryDirectory);
    mDirectoryContentsList->setDirectory(File(mCurrentLibraryPath), true, true);
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
    mFilteredFilePaths.clear();
    
    for (SampleItem* sampleItem : mAllSampleItems)
    {
        if (mFileFilter->matchesRules(*sampleItem))
        {
            mFilteredSampleItems.add(sampleItem);
            mFilteredFilePaths.add(sampleItem->getFilePath());
        }
    }
    
    sendSynchronousChangeMessage();
}

String SampleLibrary::getCurrentLibraryPath()
{
    return mCurrentLibraryPath;
}

void SampleLibrary::clearSampleItemCollections()
{
    mPaletteSampleItems.clear(false);
    mFilteredSampleItems.clear(false);
    mFilteredFilePaths.clear();
    mAllSampleItems.clear();
    sendSynchronousChangeMessage();
}

void SampleLibrary::changeListenerCallback(ChangeBroadcaster* inSource)
{
    // Set new directory if directory changes
    if (inSource == mDirectoryContentsList.get())
    {
        mDirectoryPathToAddFilesTo = mDirectoryContentsList->getDirectory().getFullPathName();
    }
    else if (inSource == mSampleLibraryManager.get())
    {
        refresh();
    }
}

void SampleLibrary::synchWithLibraryDirectory()
{
    mSampleLibraryManager->synchWithLibraryDirectory();
}

void SampleLibrary::reanalyseSampleItem(File const & inFile)
{
    // Delete sample item
    SampleItem* itemToReanalyse = mSampleLibraryManager->getSampleItemWithFilePath(inFile.getFullPathName());
    mSampleLibraryManager->analyseSampleItem(*itemToReanalyse, inFile);
    refresh();
}

StringArray& SampleLibrary::getFilteredFilePaths()
{
    return mFilteredFilePaths;
}
