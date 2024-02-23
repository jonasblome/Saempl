/*
 ==============================================================================
 
 SampleLibrary.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleLibrary.h"

SampleLibrary::SampleLibrary()
{
    mLibraryWasLoaded = false;
    mLibraryWasAltered = false;
    
    // Initialise library manager
    mSampleLibraryManager = std::make_unique<SampleLibraryManager>(mAllSampleItems, mFavouritesSampleItems, mDeletedSampleItems, mAddedSampleItems, mAlteredSampleItems);
    mSampleLibraryManager->addChangeListener(this);
    
    // Create thread for scanning the sample library directory
    mDirectoryScannerThread = std::make_unique<TimeSliceThread>("DirectoryReaderThread");
    mDirectoryScannerThread->startThread(Thread::Priority::normal);
    
    // Set file filter
    mFileFilter = std::make_unique<SampleFileFilter>("AudioFormatsFilter", mFilteredSampleItems);
    mDirectoryContentsList = std::make_unique<DirectoryContentsList>(&*mFileFilter, *mDirectoryScannerThread);
    mDirectoryContentsList->addChangeListener(this);
}

SampleLibrary::~SampleLibrary()
{
    mSampleLibraryManager->removeChangeListener(this);
    mDirectoryContentsList->removeChangeListener(this);
    
    if (mLibraryWasAltered)
    {
        mSampleLibraryManager->updateSampleLibraryFile();
    }
    
    clearSampleItemCollections();
    mDirectoryScannerThread->stopThread(10000);
}

void SampleLibrary::addAllToSampleItems(StringArray const & inFilePaths)
{
    bool anySampleWasAdded = false;
    
    for (String path : inFilePaths)
    {
        if (addFileToSampleItems(path))
        {
            anySampleWasAdded = true;
        }
    }
    
    if (anySampleWasAdded)
    {
        refresh();
    }
}

void SampleLibrary::removeSampleItems(StringArray const & inFilePaths, bool deletePermanently)
{
    for (String path : inFilePaths)
    {
        removeSampleItem(path, deletePermanently);
    }
    
    refresh();
}

void SampleLibrary::addAllToFavourites(StringArray const & inFilePaths)
{
    bool newItemCreated = false;
    
    for (String path : inFilePaths)
    {
        if (addFileToFavourites(path))
        {
            newItemCreated = true;
        }
    }
    
    if (newItemCreated)
    {
        refresh();
    }
}

void SampleLibrary::removeAllFromFavourites(Array<SampleItem*> inSampleItems)
{
    for (SampleItem* sampleItem: inSampleItems)
    {
        removeFromFavourites(*sampleItem);
    }
}

void SampleLibrary::reanalyseSampleItems(StringArray const & inFilePaths)
{
    for (String path : inFilePaths)
    {
        reanalyseSampleItem(path);
    }
    
    refresh();
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
    // If a library is currently loaded, store it
    if (mCurrentLibraryPath != "" && mLibraryWasAltered)
    {
        mSampleLibraryManager->updateSampleLibraryFile();
    }
    
    mCurrentLibraryPath = inDirectoryPath;
    File currentLibraryDirectory = File(mCurrentLibraryPath);
    
    if (!currentLibraryDirectory.exists())
    {
        currentLibraryDirectory.createDirectory();
    }
    
    // Load new library
    mLibraryWasLoaded = false;
    mLibraryWasAltered = false;
    clearSampleItemCollections();
    mSampleLibraryManager->loadSampleLibrary(currentLibraryDirectory);
    mDirectoryContentsList->setDirectory(currentLibraryDirectory, true, true);
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
        case FAVOURITE_SAMPLES:
            return mFavouritesSampleItems;
            break;
        default:
            jassertfalse;
            break;
    }
}

String SampleLibrary::getCurrentLibraryPath()
{
    return mCurrentLibraryPath;
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
        // Restore sample favourites state
        if (mRestoredFavouritesPaths.size() > 0)
        {
            for (String path : mRestoredFavouritesPaths)
            {
                File favouritesItem = File(path);
                
                if (favouritesItem.exists())
                {
                    addToFavourites(favouritesItem);
                }
            }
        }
        
        mRestoredFavouritesPaths.clear();
        
        // Refresh library
        mLibraryWasLoaded = true;
        refresh();
    }
}

void SampleLibrary::synchWithLibraryDirectory()
{
    mSampleLibraryManager->synchWithLibraryDirectory();
}

StringArray& SampleLibrary::getFilteredFilePaths()
{
    return mFilteredFilePaths;
}

void SampleLibrary::clearSampleItemCollections()
{
    mFavouritesSampleItems.clear(false);
    mFilteredSampleItems.clear(false);
    mFilteredFilePaths.clear();
    mAllSampleItems.clear();
    sendSynchronousChangeMessage();
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

bool SampleLibrary::addFileToSampleItems(File const & inFile)
{
    String fileName = inFile.getFileName();
    File newFile = File(mDirectoryPathToAddFilesTo + DIRECTORY_SEPARATOR + fileName);
    
    // Don't add files if they already exist in the current library
    if (mSampleLibraryManager->fileHasBeenAdded(inFile.getFullPathName().convertToPrecomposedUnicode()))
    {
        return false;
    }
    else if (mSampleLibraryManager->fileHasBeenAdded(newFile.getFullPathName().convertToPrecomposedUnicode()))
    {
        return false;
    }
    
    // Check directory recursively for audio files and subdirectories
    if (inFile.isDirectory())
    {
        mDirectoryPathToAddFilesTo = newFile.getFullPathName();
        bool newDirectoryWasCreated = newFile.createDirectory();
        bool anySampleWasAdded = false;
        
        if (newDirectoryWasCreated)
        {
            for (DirectoryEntry entry : RangedDirectoryIterator(inFile, false, "*", File::findFilesAndDirectories))
            {
                if (addFileToSampleItems(entry.getFile()))
                {
                    anySampleWasAdded = true;
                }
            }
        }
        
        mDirectoryPathToAddFilesTo = newFile.getParentDirectory().getFullPathName();
        return anySampleWasAdded;
    }
    else if (isSupportedAudioFileFormat(newFile.getFileExtension()))
    {
        addToSampleItems(inFile);
        return true;
    }
    
    return false;
}

SampleItem* SampleLibrary::addToSampleItems(File const & inFile)
{
    // Add file to current directory and create SampleItem
    String fileName = inFile.getFileName();
    File newFile = File(mDirectoryPathToAddFilesTo + DIRECTORY_SEPARATOR + fileName);
    inFile.copyFileTo(newFile);
    mLibraryWasAltered = true;
    SampleItem* addedItem = mSampleLibraryManager->createSampleItem(newFile);
    mAddedSampleItems.add(addedItem);
    
    return addedItem;
}

bool SampleLibrary::addFileToFavourites(File const & inFile)
{
    bool newItemCreated = false;
    
    if (inFile.isDirectory())
    {
        Array<File> sampleFiles = inFile.findChildFiles(File::findFiles, true, SUPPORTED_AUDIO_FORMATS_WILDCARD);
        
        for (File sampleFile : sampleFiles)
        {
            if (addToFavourites(sampleFile))
            {
                newItemCreated = true;
            }
        }
    }
    else if (isSupportedAudioFileFormat(inFile.getFileExtension()))
    {
        return addToFavourites(inFile);
    }
    
    return newItemCreated;
}

bool SampleLibrary::addToFavourites(File const & inFile)
{
    SampleItem* itemToAdd = mSampleLibraryManager->getSampleItemWithFilePath(inFile.getFullPathName());
    
    // Create item if it doesn't yet exist
    if (itemToAdd == nullptr)
    {
        itemToAdd = addToSampleItems(inFile);
        mFavouritesSampleItems.add(itemToAdd);
        return true;
    }
    
    // Check if item is already in favourites
    if (!mFavouritesSampleItems.contains(itemToAdd))
    {
        mFavouritesSampleItems.add(itemToAdd);
    }
    
    return false;
}

void SampleLibrary::removeSampleItem(String const & inFilePath, bool deletePermanently = false)
{
    File fileToDelete = File(inFilePath);
    
    // Delete sample item
    SampleItem* itemToDelete = mSampleLibraryManager->getSampleItemWithFilePath(fileToDelete.getFullPathName());
    mDeletedSampleItems.add(itemToDelete);
    removeFromFavourites(*itemToDelete);
    mAllSampleItems.removeObject(itemToDelete, false);
    
    // Delete audio file
    if (fileToDelete.exists())
    {
        deletePermanently ? fileToDelete.deleteRecursively() : fileToDelete.moveToTrash();
    }
    
    mLibraryWasAltered = true;
}

void SampleLibrary::removeFromFavourites(SampleItem& inSampleItem)
{
    mFavouritesSampleItems.removeObject(&inSampleItem, false);
}

void SampleLibrary::reanalyseSampleItem(File const & inFile)
{
    // Delete sample item
    if (inFile.isDirectory())
    {
        Array<File> sampleFiles = inFile.findChildFiles(File::findFiles, true, SUPPORTED_AUDIO_FORMATS_WILDCARD);
        
        for (File sampleFile : sampleFiles)
        {
            reanalyseSampleItem(sampleFile);
        }
    }
    else
    {
        SampleItem* itemToReanalyse = mSampleLibraryManager->getSampleItemWithFilePath(inFile.getFullPathName());
        mSampleLibraryManager->analyseSampleItem(itemToReanalyse, inFile, true);
        mAlteredSampleItems.add(itemToReanalyse);
        mLibraryWasAltered = true;
    }
}

bool SampleLibrary::getLibraryWasLoaded()
{
    return mLibraryWasLoaded;
}

void SampleLibrary::setRestoredFavouritesPaths(StringArray inRestoredFavouritesPaths)
{
    mRestoredFavouritesPaths = inRestoredFavouritesPaths;
}
