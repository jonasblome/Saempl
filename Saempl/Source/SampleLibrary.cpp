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
    mLibraryFilter = std::make_unique<SampleFileFilter>("AudioFormatsFilter", mFilteredSampleItems);
    mDirectoryContent = std::make_unique<DirectoryContentsList>(&*mLibraryFilter, inThread);
    
    // Set directory
    mDirectoryContent->addChangeListener(this);
    setDirectory((File::getSpecialLocation(File::userMusicDirectory)).getFullPathName()
                 + DIRECTORY_SEPARATOR
                 + "Plugins"
                 + DIRECTORY_SEPARATOR
                 + "Saempl"
                 + DIRECTORY_SEPARATOR
                 + "DefaultSampleLibrary");
}

SampleLibrary::~SampleLibrary()
{
    mDirectoryContent->removeChangeListener(this);
    mSampleLibraryManager->updateSampleLibraryFile(mCurrentLibraryPath, mAllSampleItems);
}

void SampleLibrary::addToSampleItems(File const & inFile)
{
    String fileName = inFile.getFileName();
    File newFile = File(mDirectoryPathToAddFilesTo + DIRECTORY_SEPARATOR + fileName);
    
    // Don't add files if they already exist in the current library
    if (!mDirectoryContent->getDirectory().findChildFiles(File::findFiles, true, newFile.getFileName()).isEmpty())
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
    SampleItem* itemToAdd = getSampleItemWithFileName(inFile.getFileName());
    
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
    removeSampleItemFromPalette(*itemToDelete);
    
    // Delete audio file
    File fileToDelete = File(inFilePath);
    
    if (fileToDelete.exists())
    {
        deletePermanently ? fileToDelete.deleteRecursively() : fileToDelete.moveToTrash();
    }
}

void SampleLibrary::removeSampleItemFromPalette(SampleItem& inSampleItem)
{
    mPaletteSampleItems.removeObject(&inSampleItem, false);
}

DirectoryContentsList& SampleLibrary::getDirectoryList()
{
    return *mDirectoryContent;
}

SampleFileFilter& SampleLibrary::getFileFilter()
{
    return *mLibraryFilter;
}

void SampleLibrary::changeListenerCallback(ChangeBroadcaster* inSource)
{
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
    // Go through all current sample items, check if corresponding audio file still exists and if not, delete sample item
    for (SampleItem* sampleItem : mAllSampleItems)
    {
        if (!File(sampleItem->getFilePath()).exists())
        {
            mPaletteSampleItems.removeObject(sampleItem, false);
            mAllSampleItems.removeObject(sampleItem);
        }
    }
    
    // Go through all files in directory, check if a corresponding sample item already exists in the sample item list, if not add it
    for (DirectoryEntry entry : RangedDirectoryIterator(mDirectoryContent->getDirectory(),
                                                        true,
                                                        SUPPORTED_AUDIO_FORMATS_WILDCARD,
                                                        File::findFiles))
    {
        bool linkedSampleItemExists = getSampleItemWithFileName(entry.getFile().getFileName()) != nullptr;
        
        if (!linkedSampleItemExists)
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
        if (File(sampleItem->getFilePath()).getFileName() == inFileName)
        {
            return sampleItem;
        }
    }
    
    return nullptr;
}

void SampleLibrary::setDirectory(String inDirectoryPath)
{
    if (inDirectoryPath != "")
    {
        // Store current library
        mSampleLibraryManager->updateSampleLibraryFile(mCurrentLibraryPath, mAllSampleItems);
    }
    
    mCurrentLibraryPath = inDirectoryPath;
    
    if (!File(mCurrentLibraryPath).exists())
    {
        File(mCurrentLibraryPath).createDirectory();
    }
    
    // Load new library
    mFilteredSampleItems.clear(false);
    mAllSampleItems.clear();
    mSampleLibraryManager->loadSampleLibraryFile(mCurrentLibraryPath, mAllSampleItems);
    mDirectoryContent->setDirectory(File(mCurrentLibraryPath), true, true);
    refresh();
}

OwnedArray<SampleItem>& SampleLibrary::getSampleItems(SampleItemCollectionType inCollectionType)
{
    switch (inCollectionType)
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
            break;
    }
}

/**
 Creates a SampleItem for a file and adds it to the collection.
 If the corresponding SampleItem already exists, nothing happens.
 */
void SampleLibrary::createSampleItem(File inFile)
{
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
        if (mLibraryFilter->matchesRules(*sampleItem))
        {
            mFilteredSampleItems.add(sampleItem);
        }
    }
    
    mLibraryFilter->setFilteredSampleItems(mFilteredSampleItems);
}

String SampleLibrary::getCurrentLibraryPath()
{
    return mCurrentLibraryPath;
}
